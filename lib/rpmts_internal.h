#ifndef _RPMTS_INTERNAL_H
#define _RPMTS_INTERNAL_H

#include <rpm/rpmts.h>

#include "lib/rpmal.h"		/* XXX availablePackage */
#include "lib/rpmhash.h"	/* XXX hashTable */
#include "lib/fprint.h"

typedef struct diskspaceInfo_s * rpmDiskSpaceInfo;

/* Transaction set elements information */
typedef struct tsMembers_s {
    int * removedPackages;	/*!< Set of packages being removed. */
    int numRemovedPackages;	/*!< No. removed package instances. */
    int allocedRemovedPackages;	/*!< Size of removed packages array. */

    rpmal addedPackages;	/*!< Set of packages being installed. */
    int numAddedPackages;	/*!< No. added package instances. */

    rpmte * order;		/*!< Packages sorted by dependencies. */
    int orderCount;		/*!< No. of transaction elements. */
    int orderAlloced;		/*!< No. of allocated transaction elements. */
    int delta;			/*!< Delta for reallocation. */
} * tsMembers;

/** \ingroup rpmts
 * The set of packages to be installed/removed atomically.
 */
struct rpmts_s {
    rpmtransFlags transFlags;	/*!< Bit(s) to control operation. */

    int (*solve) (rpmts ts, rpmds key, const void * data);
                                /*!< Search for NEVRA key. */
    const void * solveData;	/*!< Solve callback data */

    rpmCallbackFunction notify;	/*!< Callback function. */
    rpmCallbackData notifyData;	/*!< Callback private data. */

    rpmprobFilterFlags ignoreSet;
				/*!< Bits to filter current problems. */

    rpmDiskSpaceInfo dsi;	/*!< Per filesystem disk/inode usage. */

    rpmdb rdb;			/*!< Install database handle. */
    int dbmode;			/*!< Install database open mode. */

    tsMembers members;		/*!< Transaction set member info (order etc) */

    int selinuxEnabled;		/*!< Is SE linux enabled? */
    int chrootDone;		/*!< Has chroot(2) been been done? */
    char * rootDir;		/*!< Path to top of install tree. */
    char * currDir;		/*!< Current working directory. */
    FD_t scriptFd;		/*!< Scriptlet stdout/stderr. */
    rpm_tid_t tid;		/*!< Transaction id. */

    rpm_color_t color;		/*!< Transaction color bits. */
    rpm_color_t prefcolor;	/*!< Preferred file color. */

    rpmVSFlags vsflags;		/*!< Signature/digest verification flags. */

    rpmKeyring keyring;		/*!< Keyring in use. */

    ARGV_t netsharedPaths;	/*!< From %{_netsharedpath} */
    ARGV_t installLangs;	/*!< From %{_install_langs} */

    struct rpmop_s ops[RPMTS_OP_MAX];

    rpmSpec spec;		/*!< Spec file control structure. */

    int nrefs;			/*!< Reference count. */
};

RPM_GNUC_INTERNAL
tsMembers rpmtsMembers(rpmts ts);

#endif /* _RPMTS_INTERNAL_H */
