/*
 * project-conf.h
 * Umay-6T Global Configuration Parameters
 * Place this in your simulation or application directory (e.g., alongside Makefile).
 */

 * \author
 *         Hakan AYDIN <hakayd28@gmail.com>
 */
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

/* Umay-6T Protocol Switch */
#define UMAY_6T_ENABLED 1

/* Parameters from Eq. (2) in Article - Adaptive Cache Model */
#define UMAY_MIN_CACHE_DURATION  3   /* T_min: 3 slotframes base */
#define UMAY_DEFAULT_CACHE_DURATION 60 /* Fallback baseline duration */
#define UMAY_ALPHA 1.2                 /* Extrapolated as 1.2 in floating point mapping */
#define UMAY_BETA 0.5                  /* Extrapolated as 0.5 in floating point mapping */

/* Advanced Robustness Switches */
#define UMAY_CONSISTENCY_CHECK_ENABLED 1 /* FRM State Verification: Prevents "Zombie" cells */
#define UMAY_RPL_SYNC_ENABLED 1          /* Cross-layer optimization: Immediate routing update */

/* Internal Cell States for Umay-6T CRM */
#define CELL_STATE_ACTIVE 0
#define CELL_STATE_CACHED 1
#define CELL_STATE_VALIDATING 2 /* Wait for 6P COUNT Response from returning neighbor */

#endif /* PROJECT_CONF_H_ */
