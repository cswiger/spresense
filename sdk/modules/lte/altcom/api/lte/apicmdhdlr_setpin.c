/****************************************************************************
 * modules/lte/altcom/api/lte/apicmdhdlr_setpin.c
 *
 *   Copyright 2018 Sony Semiconductor Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Sony Semiconductor Solutions Corporation nor
 *    the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <string.h>

#include "lte/lte_api.h"
#include "buffpoolwrapper.h"
#include "apicmd_setpinlock.h"
#include "apicmd_setpincode.h"
#include "evthdlbs.h"
#include "apicmdhdlrbs.h"

/****************************************************************************
 * Public Data
 ****************************************************************************/

extern set_pinenable_cb_t g_pinenable_callback;
extern change_pin_cb_t    g_changepin_callback;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: setpin_lock_job
 *
 * Description:
 *   This function is an API callback for set PIN lock.
 *
 * Input Parameters:
 *  arg    Pointer to received event.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

static void setpin_lock_job(FAR void *arg)
{
  int32_t                                  ret                = -1;
  set_pinenable_cb_t                       pinenable_callback = NULL;
  FAR struct apicmd_cmddat_setpinlockres_s *resdat            = NULL;

  resdat = (FAR struct apicmd_cmddat_setpinlockres_s *)arg;
  if (APICMD_SETPINLOCK_RES_OK > resdat->result ||
      APICMD_SETPINLOCK_RES_ERR < resdat->result)
    {
      DBGIF_ASSERT(NULL, "Invalid response.\n");
    }

  ALTCOM_GET_AND_CLR_CALLBACK(
    ret, g_pinenable_callback, pinenable_callback);

  if (0 == ret && pinenable_callback)
    {
      pinenable_callback(resdat->result, resdat->attemptsleft);
    }
  else
    {
      DBGIF_LOG_ERROR("Unexpected!! callback is NULL.\n");
    }

  altcom_free_cmd((FAR uint8_t *)arg);
}

/****************************************************************************
 * Name: setpin_code_job
 *
 * Description:
 *   This function is an API callback for set PIN code.
 *
 * Input Parameters:
 *  arg    Pointer to received event.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

static void setpin_code_job(FAR void *arg)
{
  int32_t                                  ret                = -1;
  change_pin_cb_t                          changepin_callback = NULL;
  FAR struct apicmd_cmddat_setpincoderes_s *resdat            = NULL;

  resdat = (FAR struct apicmd_cmddat_setpincoderes_s *)arg;
  if (APICMD_SETPINCODE_RES_OK > resdat->result ||
      APICMD_SETPINCODE_RES_ERR < resdat->result)
    {
      DBGIF_ASSERT(NULL, "Invalid response.\n");
    }

  ALTCOM_GET_AND_CLR_CALLBACK(
    ret, g_changepin_callback, changepin_callback);
  if (0 == ret && changepin_callback)
    {
      changepin_callback(resdat->result, resdat->attemptsleft);
    }
  else
    {
      DBGIF_LOG_ERROR("Unexpected!! callback is NULL.\n");
    }

  altcom_free_cmd((FAR uint8_t *)arg);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: apicmdhdlr_setpin
 *
 * Description:
 *   This function is an API command handler for set PIN set result.
 *
 * Input Parameters:
 *  evt    Pointer to received event.
 *  evlen  Length of received event.
 *
 * Returned Value:
 *   If the API command ID matches APICMDID_SET_PIN_LOCK_RES or
 *   APICMDID_SET_PIN_CODE_RES, EVTHDLRC_STARTHANDLE is returned.
 *   Otherwise it returns EVTHDLRC_UNSUPPORTEDEVENT. If an internal error is
 *   detected, EVTHDLRC_INTERNALERROR is returned.
 *
 ****************************************************************************/

enum evthdlrc_e apicmdhdlr_setpin(FAR uint8_t *evt, uint32_t evlen)
{
  enum evthdlrc_e ret;

  ret = apicmdhdlrbs_do_runjob(
    evt, APICMDID_CONVERT_RES(APICMDID_SET_PIN_LOCK), setpin_lock_job);
  if (EVTHDLRC_UNSUPPORTEDEVENT == ret)
    {
      ret = apicmdhdlrbs_do_runjob(
        evt, APICMDID_CONVERT_RES(APICMDID_SET_PIN_CODE), setpin_code_job);
    }

  return ret;
}
