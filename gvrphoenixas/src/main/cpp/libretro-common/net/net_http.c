/* Copyright  (C) 2010-2017 The RetroArch team
 *
 * ---------------------------------------------------------------------------------------
 * The following license statement only applies to this file (net_http.c).
 * ---------------------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <net/net_http.h>
#include <net/net_compat.h>
#include <net/net_socket.h>
#include <compat/strl.h>
#include <string/stdstring.h>

enum
{
   P_HEADER_TOP = 0,
   P_HEADER,
   P_BODY,
   P_BODY_CHUNKLEN,
   P_DONE,
   P_ERROR
};

enum
{
   T_FULL = 0,
   T_LEN,
   T_CHUNK
};

struct http_t
{
   int fd;
   int status;
   
   char part;
   char bodytype;
   bool error;
   
   size_t pos;
   size_t len;
   size_t buflen;
   char * data;
};

struct http_connection_t
{
   char *domain;
   char *location;
   char *urlcopy;
   char *scan;
   char *methodcopy;
   char *contenttypecopy;
   char *postdatacopy;
   int port;
};

static char urlencode_lut[256];
static bool urlencode_lut_inited = false;

void urlencode_lut_init()
{
   unsigned i;

   urlencode_lut_inited = true;

   for (i = 0; i < 256; i++)
   {
      urlencode_lut[i] = isalnum(i) || i == '*' || i == '-' || i == '.' || i == '_' ? i : (i == ' ') ? '+' : 0;
   }
}

/* caller is responsible for deleting the destination buffer */
void net_http_urlencode_full(char **dest, const char *source)
{
   char *enc  = NULL;
   /* Assume every character will be encoded, so we need 3 times the space. */
   size_t len = strlen(source) * 3 + 1;

   if (!urlencode_lut_inited)
      urlencode_lut_init();

   enc = (char*)calloc(1, len);

   *dest = enc;

   for (; *source; source++)
   {
      /* any non-ascii character will just be encoded without question */
      if ((int)*source < sizeof(urlencode_lut) && urlencode_lut[(int)*source])
         snprintf(enc, len, "%c", urlencode_lut[(int)*source]);
      else
         snprintf(enc, len, "%%%02X", *source & 0xFF);

      while (*++enc);
   }

   (*dest)[len - 1] = '\0';
}

static int net_http_new_socket(const char *domain, int port)
{
   int ret;
   struct addrinfo *addr = NULL;
   int fd                = socket_init(
         (void**)&addr, port, domain, SOCKET_TYPE_STREAM);

   if (fd < 0)
      return -1;

   ret = socket_connect(fd, (void*)addr, true);

   freeaddrinfo_retro(addr);

   if (ret < 0)
      goto error;

   if (!socket_nonblock(fd))
      goto error;

   return fd;

error:
   socket_close(fd);
   return -1;
}

static void net_http_send_str(int fd, bool *error, const char *text)
{
   if (*error)
      return;

   if (!socket_send_all_blocking(fd, text, strlen(text), true))
      *error = true;
}

struct http_connection_t *net_http_connection_new(const char *url,
      const char *method, const char *data)
{
   char **domain = NULL;
   struct http_connection_t *conn = (struct http_connection_t*)calloc(1,
         sizeof(struct http_connection_t));

   if (!conn)
      return NULL;

   if (!url)
      goto error;

   conn->urlcopy         = strdup(url);

   if (method)
      conn->methodcopy   = strdup(method);

   if (data)
      conn->postdatacopy = strdup(data);

   if (!conn->urlcopy)
      goto error;

   if (strncmp(url, "http://", strlen("http://")) != 0)
      goto error;

   conn->scan    = conn->urlcopy + strlen("http://");
   domain        = &conn->domain;
   *domain       = conn->scan;

   return conn;

error:
   if (conn->urlcopy)
      free(conn->urlcopy);
   if (conn->methodcopy)
      free(conn->methodcopy);
   if (conn->postdatacopy)
      free(conn->postdatacopy);
   conn->urlcopy      = NULL;
   conn->methodcopy   = NULL;
   conn->postdatacopy = NULL;
   free(conn);
   return NULL;
}

bool net_http_connection_iterate(struct http_connection_t *conn)
{
   if (!conn)
      return false;

   while (*conn->scan != '/' && *conn->scan != ':' && *conn->scan != '\0')
      conn->scan++;

   return true;
}

bool net_http_connection_done(struct http_connection_t *conn)
{
   char **location = NULL;

   if (!conn)
      return false;

   location     = &conn->location;

   if (*conn->scan == '\0')
      return false;

   *conn->scan  = '\0';
   conn->port   = 80;

   if (*conn->scan == ':')
   {
      if (!isdigit((int)conn->scan[1]))
         return false;

      conn->port = (int)strtoul(conn->scan + 1, &conn->scan, 10);

      if (*conn->scan != '/')
         return false;
   }

   *location = conn->scan + 1;

   return true;
}

void net_http_connection_free(struct http_connection_t *conn)
{
   if (!conn)
      return;

   if (conn->urlcopy)
      free(conn->urlcopy);

   if (conn->methodcopy)
      free(conn->methodcopy);

   if (conn->contenttypecopy)
      free(conn->contenttypecopy);

   if (conn->postdatacopy)
      free(conn->postdatacopy);

   conn->urlcopy = NULL;
   conn->methodcopy = NULL;
   conn->contenttypecopy = NULL;
   conn->postdatacopy = NULL;

   free(conn);
}

const char *net_http_connection_url(struct http_connection_t *conn)
{
   return conn->urlcopy;
}

struct http_t *net_http_new(struct http_connection_t *conn)
{
   bool error            = false;
   int fd                = -1;
   struct http_t *state  = NULL;

   if (!conn)
      goto error;

   fd = net_http_new_socket(conn->domain, conn->port);
   if (fd < 0)
      goto error;

   error = false;

   /* This is a bit lazy, but it works. */
   if (conn->methodcopy)
   {
      net_http_send_str(fd, &error, conn->methodcopy);
      net_http_send_str(fd, &error, " /");
   }
   else
   {
      net_http_send_str(fd, &error, "GET /");
   }

   net_http_send_str(fd, &error, conn->location);
   net_http_send_str(fd, &error, " HTTP/1.1\r\n");

   net_http_send_str(fd, &error, "Host: ");
   net_http_send_str(fd, &error, conn->domain);

   if (conn->port != 80)
   {
      char portstr[16];

      portstr[0] = '\0';

      snprintf(portstr, sizeof(portstr), ":%i", conn->port);
      net_http_send_str(fd, &error, portstr);
   }

   net_http_send_str(fd, &error, "\r\n");

   /* this is not being set anywhere yet */
   if (conn->contenttypecopy)
   {
      net_http_send_str(fd, &error, "Content-Type: ");
      net_http_send_str(fd, &error, conn->contenttypecopy);
      net_http_send_str(fd, &error, "\r\n");
   }

   if (conn->methodcopy && (string_is_equal_fast(conn->methodcopy, "POST", 4)))
   {
      size_t post_len, len;
      char *len_str        = NULL;

      if (!conn->postdatacopy)
         goto error;

      if (!conn->contenttypecopy)
         net_http_send_str(fd, &error,
               "Content-Type: application/x-www-form-urlencoded\r\n");

      net_http_send_str(fd, &error, "Content-Length: ");

      post_len = strlen(conn->postdatacopy);
#ifdef _WIN32
      len = snprintf(NULL, 0, "%I64u", post_len);
      len_str = (char*)malloc(len + 1);
      snprintf(len_str, len + 1, "%I64u", post_len);
#else
      len = snprintf(NULL, 0, "%llu", (long long unsigned)post_len);
      len_str = (char*)malloc(len + 1);
      snprintf(len_str, len + 1, "%llu", (long long unsigned)post_len);
#endif

      len_str[len] = '\0';

      net_http_send_str(fd, &error, len_str);
      net_http_send_str(fd, &error, "\r\n");

      free(len_str);
   }

   net_http_send_str(fd, &error, "User-Agent: libretro\r\n");
   net_http_send_str(fd, &error, "Connection: close\r\n");
   net_http_send_str(fd, &error, "\r\n");

   if (conn->methodcopy && (string_is_equal_fast(conn->methodcopy, "POST", 4)))
      net_http_send_str(fd, &error, conn->postdatacopy);

   if (error)
      goto error;

   state          = (struct http_t*)malloc(sizeof(struct http_t));
   state->fd      = fd;
   state->status  = -1;
   state->data    = NULL;
   state->part    = P_HEADER_TOP;
   state->bodytype= T_FULL;
   state->error   = false;
   state->pos     = 0;
   state->len     = 0;
   state->buflen  = 512;
   state->data    = (char*)malloc(state->buflen);

   if (!state->data)
      goto error;

   return state;

error:
   if (conn->methodcopy)
      free(conn->methodcopy);
   if (conn->contenttypecopy)
      free(conn->contenttypecopy);
   conn->methodcopy = NULL;
   conn->contenttypecopy = NULL;
   conn->postdatacopy = NULL;
   if (fd >= 0)
      socket_close(fd);
   if (state)
      free(state);
   return NULL;
}

int net_http_fd(struct http_t *state)
{
   if (!state)
      return -1;
   return state->fd;
}

bool net_http_update(struct http_t *state, size_t* progress, size_t* total)
{
   ssize_t newlen = 0;

   if (!state || state->error)
      goto fail;

   if (state->part < P_BODY)
   {
      if (state->error)
         newlen = -1;
      else
         newlen = socket_receive_all_nonblocking(state->fd, &state->error,
               (uint8_t*)state->data + state->pos,
               state->buflen - state->pos);

      if (newlen < 0)
         goto fail;

      if (state->pos + newlen >= state->buflen - 64)
      {
         state->buflen *= 2;
         state->data    = (char*)realloc(state->data, state->buflen);
      }
      state->pos += newlen;

      while (state->part < P_BODY)
      {
         char *dataend = state->data + state->pos;
         char *lineend = (char*)memchr(state->data, '\n', state->pos);

         if (!lineend)
            break;

         *lineend='\0';

         if (lineend != state->data && lineend[-1]=='\r')
            lineend[-1]='\0';

         if (state->part == P_HEADER_TOP)
         {
            if (strncmp(state->data, "HTTP/1.", strlen("HTTP/1."))!=0)
               goto fail;
            state->status = (int)strtoul(state->data + strlen("HTTP/1.1 "), NULL, 10);
            state->part   = P_HEADER;
         }
         else
         {
            if (!strncmp(state->data, "Content-Length: ",
                     strlen("Content-Length: ")))
            {
               state->bodytype = T_LEN;
               state->len = strtol(state->data + 
                     strlen("Content-Length: "), NULL, 10);
            }
            if (string_is_equal(state->data, "Transfer-Encoding: chunked"))
               state->bodytype = T_CHUNK;

            /* TODO: save headers somewhere */
            if (state->data[0]=='\0')
            {
               state->part = P_BODY;
               if (state->bodytype == T_CHUNK)
                  state->part = P_BODY_CHUNKLEN;
            }
         }

         memmove(state->data, lineend + 1, dataend-(lineend+1));
         state->pos = (dataend-(lineend + 1));
      }
      if (state->part >= P_BODY)
      {
         newlen = state->pos;
         state->pos = 0;
      }
   }

   if (state->part >= P_BODY && state->part < P_DONE)
   {
      if (!newlen)
      {
         if (state->error)
            newlen = -1;
         else
            newlen = socket_receive_all_nonblocking(
                  state->fd,
                  &state->error,
                  (uint8_t*)state->data + state->pos,
                  state->buflen - state->pos);

         if (newlen < 0)
         {
            if (state->bodytype == T_FULL)
            {
               state->part = P_DONE;
               state->data = (char*)realloc(state->data, state->len);
            }
            else
               goto fail;
            newlen=0;
         }

         if (state->pos + newlen >= state->buflen - 64)
         {
            state->buflen *= 2;
            state->data = (char*)realloc(state->data, state->buflen);
         }
      }

parse_again:
      if (state->bodytype == T_CHUNK)
      {
         if (state->part == P_BODY_CHUNKLEN)
         {
            state->pos += newlen;
            if (state->pos - state->len >= 2)
            {
               /*
                * len=start of chunk including \r\n
                * pos=end of data
                */

               char *fullend = state->data + state->pos;
               char *end     = (char*)memchr(state->data + state->len + 2, '\n',
                                             state->pos - state->len - 2);

               if (end)
               {
                  size_t chunklen = strtoul(state->data+state->len, NULL, 16);
                  state->pos      = state->len;
                  end++;

                  memmove(state->data+state->len, end, fullend-end);

                  state->len      = chunklen;
                  newlen          = (fullend - end);

                  /*
                     len=num bytes
                     newlen=unparsed bytes after \n
                     pos=start of chunk including \r\n
                     */

                  state->part = P_BODY;
                  if (state->len == 0)
                  {
                     state->part = P_DONE;
                     state->len  = state->pos;
                     state->data = (char*)realloc(state->data, state->len);
                  }
                  goto parse_again;
               }
            }
         }
         else if (state->part == P_BODY)
         {
            if ((size_t)newlen >= state->len)
            {
               state->pos += state->len;
               newlen     -= state->len;
               state->len  = state->pos;
               state->part = P_BODY_CHUNKLEN;
               goto parse_again;
            }
            else
            {
               state->pos += newlen;
               state->len -= newlen;
            }
         }
      }
      else
      {
         state->pos += newlen;

         if (state->pos == state->len)
         {
            state->part = P_DONE;
            state->data = (char*)realloc(state->data, state->len);
         }
         if (state->pos > state->len)
            goto fail;
      }
   }

   if (progress)
      *progress = state->pos;

   if (total)
   {
      if (state->bodytype == T_LEN)
         *total=state->len;
      else
         *total=0;
   }

   return (state->part == P_DONE);

fail:
   if (state)
   {
      state->error  = true;
      state->part   = P_ERROR;
      state->status = -1;
   }

   return true;
}

int net_http_status(struct http_t *state)
{
   if (!state)
      return -1;
   return state->status;
}

uint8_t* net_http_data(struct http_t *state, size_t* len, bool accept_error)
{
   if (!state)
      return NULL;

   if (!accept_error && net_http_error(state))
   {
      if (len)
         *len=0;
      return NULL;
   }

   if (len)
      *len=state->len;

   return (uint8_t*)state->data;
}

void net_http_delete(struct http_t *state)
{
   if (!state)
      return;

   if (state->fd >= 0)
      socket_close(state->fd);
   free(state);
}

bool net_http_error(struct http_t *state)
{
   return (state->error || state->status<200 || state->status>299);
}
