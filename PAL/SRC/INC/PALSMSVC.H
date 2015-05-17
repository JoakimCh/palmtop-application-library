/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  sm_svc.h
   Author:  Ron Crain/Gilles Kohl
   Started: Tue 11-22-1994
   Subject: Support for SysMgr services

   Macros and prototypes for the System Manager services
   --------------------------------------------------------------------*/

/* --------------------------------------------------------------------
                      constant definitions
   -------------------------------------------------------------------- */

#define CBCODES
#define CB_OK         0      /* All's well with the world */
#define CB_LOCKED    -1      /* Clipboard access denied */
#define CB_NOT_OPEN  -2      /* Clipboard not formally open */
#define CB_ALLOC     -3      /* Heap allocation failure */
#define CB_EXISTS    -4      /* Representation already exists */
#define CB_REP_OPEN  -5      /* A Representation is open */
#define CB_NO_REP    -6      /* No representation open */
#define CB_NO_SUCH   -7      /* No such representation */
#define CB_BOUNDS    -8      /* Transfer request out of bounds */

/* Maximum size of a clipboard author or representation name */
#define MAX_CB_NAME 48

/* Declarations, of the strings only, of certain representation type names */
#define CB_REP_TEXT        "TEXT"
#define CB_REP_KEYSTROKE   "KEYSTROKE"


/* --------------------------------------------------------------------
                       macro definitions
   -------------------------------------------------------------------- */

/* Make c_service call the code that pCsvc points to */
#define c_service (*pCsvc)
#define c_service_far_ptr (*pCsvc_fp)

/* Memory block prototypes */
#define  CLASS_BASE     0x40
#define  FNCT_BASE      0x01
#define  SC_APIMGT      (CLASS_BASE + 0)
#define  SC_CB          (CLASS_BASE + 7)
#define  SC_SOUND       (CLASS_BASE + 8)
#define  SC_MM          (CLASS_BASE + 9)
#define  SC_MISC        (CLASS_BASE + 15)

#define  API_RSRC_LIB   0           /* resource lib service */
#define  API_DRIVERS    1           /* drivers              */
#define  API_SYSMGR     2           /* system manager       */
#define  API_APPS       3           /* application specific */

/* --------------------------------------------------------------------
                           Clipboard services
   -------------------------------------------------------------------- */

#define F_M_OPEN_CB     (SC_CB * 256) + 0 + FNCT_BASE
#define m_open_cb()             \
   c_service(F_M_OPEN_CB)

#define F_M_CLOSE_CB    (SC_CB * 256) + 1 + FNCT_BASE
#define m_close_cb()            \
   c_service(F_M_CLOSE_CB)

#define F_M_RESET_CB    (SC_CB * 256) + 2 + FNCT_BASE
#define m_reset_cb(a)           \
   c_service(F_M_RESET_CB,(void far *)(a))

#define F_M_GET_CB_INFO (SC_CB * 256) + 3 + FNCT_BASE
#define m_get_cb_info(count,author,cbsize,inuse)         \
   c_service(F_M_GET_CB_INFO, (void far *)(count), (void far *)(author),   \
                              (void far *)(cbsize), (void far *)(inuse))

#define F_M_NEW_REP     (SC_CB * 256) + 4 + FNCT_BASE
#define m_new_rep(a)             \
   c_service(F_M_NEW_REP,(void far *)(a))

#define F_M_FINI_REP    (SC_CB * 256) + 5 + FNCT_BASE
#define m_fini_rep()            \
   c_service(F_M_FINI_REP)

#define F_M_REP_INDEX   (SC_CB * 256) + 7 + FNCT_BASE
#define m_rep_index(a,b,c)      \
   c_service(F_M_REP_INDEX,(void far *)(a),(void far *)(b),(void far *)(c))

#define F_M_CB_WRITE    (SC_CB * 256) + 8 + FNCT_BASE
#define m_cb_write(a,b)         \
   c_service(F_M_CB_WRITE,(void far *)(a),b)

#define F_M_CB_READ     (SC_CB * 256) + 9 + FNCT_BASE
#define m_cb_read(a,b,c,d)      \
   c_service(F_M_CB_READ,a,b,(void far *)(c),d)

/* --------------------------------------------------------------------
                        Task Management services
   -------------------------------------------------------------------- */

#define F_M_GET_TCB            (SC_MM * 256) + 12 + FNCT_BASE
#define m_get_TCB()             \
   c_service_far_ptr(F_M_GET_TCB)

#define F_M_GET_TCB_SIZE (SC_MISC * 256) + 19 + FNCT_BASE
#define m_get_TCB_size()	    \
   c_service_far_ptr(F_M_GET_TCB_SIZE )


/* --------------------------------------------------------------------
                             Sound services
   -------------------------------------------------------------------- */

#define F_M_BEEP        (SC_SOUND * 256) + 0 + FNCT_BASE
#define m_beep()        c_service(F_M_BEEP)

#define F_M_THUD        (SC_SOUND * 256) + 1 + FNCT_BASE
#define m_thud()        c_service(F_M_THUD)

#define F_M_ASOUND      (SC_SOUND * 256) + 2 + FNCT_BASE
#define m_asound(a)     c_service(F_M_ASOUND,a)

#define F_M_SOUNDOFF    (SC_SOUND * 256) + 3 + FNCT_BASE
#define m_soundoff()    c_service(F_M_SOUNDOFF,a)

#define F_M_PLAY        (SC_SOUND * 256) + 4 + FNCT_BASE
#define m_play(a)	 c_service(F_M_PLAY,(void far *)(a))


/* --------------------------------------------------------------------
                           Miscellaneous
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                        type definitions
   -------------------------------------------------------------------- */

/* define a type 'pointer to a far function accepting an int
   and an arbitrary number of parameters, and returning an int',
   for the c_service call mechanism */
typedef int (far *PCSVC)(int, ...);
typedef void far * (far *PCSVC_FP)(int, ...);

/* TASK CONTROL BLOCK structure */
struct task {
   unsigned  t_sp;             /* offset made low, mdb     */
   unsigned  t_ss;
   unsigned  t_off_image;      /* offset made low, mdb     */
   unsigned  t_seg_image;
   unsigned  t_ds;
   unsigned  t_memseg;         /* base of allocation block */
   unsigned  t_hotkey;         /* copied from IMAGEVEC     */

   unsigned  t_phypage[2];     /* IMAGEVEC.wAS_PhyPage     */
   unsigned  t_logicalpage[2]; /* IMAGEVEC.wRS_LogicalPage */
   unsigned  t_pagecount[2];   /* IMAGEVEC.wRS_PageCount   */
   char      t_chipsel[2];
   char      t_state;
   char      t_is123;
   unsigned  t_rsrc_seg;
   char      t_nowait;
   char      t_sys_ver;
   char      t_extname[12];
   char      t_lock_state;
   char      t_spare[3];
   unsigned  t_far_size;
   unsigned  t_far_off;
   unsigned  t_far_rsvrd;
   char      t_sp_status;
   char      t_sp_link;
   unsigned  t_ic_u_loc;
   unsigned  t_ic_o_loc;
};

/* --------------------------------------------------------------------
                        global variables/externals
   -------------------------------------------------------------------- */

extern PCSVC pCsvc;       /* defined in csvc.c, used by c_service macro */
extern PCSVC_FP pCsvc_fp; /* defined in csvc.c, used by c_service macro */


