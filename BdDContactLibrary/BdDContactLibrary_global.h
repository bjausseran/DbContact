#ifndef BDDCONTACTLIBRARY_GLOBAL_H
#define BDDCONTACTLIBRARY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BDDCONTACTLIBRARY_LIBRARY)
#  define BDDCONTACTLIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define BDDCONTACTLIBRARY_EXPORT Q_DECL_IMPORT
#endif

#endif // BDDCONTACTLIBRARY_GLOBAL_H
