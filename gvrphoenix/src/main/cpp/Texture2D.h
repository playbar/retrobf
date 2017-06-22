#ifndef __INSTANCING_H__
#define __INSTANCING_H__

#include "android/log.h"
#include "esUtil.h"

#ifdef __cplusplus
extern "C" {
#endif

int Init( ESContext *esContext );
void Draw ( ESContext *esContext );
void ShutDown ( ESContext *esContext );

#ifdef __cplusplus
}
#endif
#endif