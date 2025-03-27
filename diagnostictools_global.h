#pragma once

#include <qglobal.h>

#if defined(DIAGNOSTICTOOLS_LIBRARY)
#define DIAGNOSTICTOOLS_EXPORT Q_DECL_EXPORT
#else
#define DIAGNOSTICTOOLS_EXPORT Q_DECL_IMPORT
#endif
