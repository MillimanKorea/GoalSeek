// User defined support functions __________________________________________________________________

// Implementation control routines _________________________________________________________________

void       Solve_mGPS(    WDATA_MGPS     *pWdata);

double     SolveA_mGPS(   WDATA_MGPS     *pWdata,
                          double          x);

// Calulation control ______________________________________________________________________________

void Calculate_mGPS(ARGS_UDF *pArgs, WDATA_MGPS *pWdata)
    {
    int                      t;
    double                   TGP0, TGP1, TMR0, TMR1;
    INPUT_MGPS              *pInput;
    WORK_MGPS               *pWork;

    pInput                = pWdata->pInput;
    pWork                 = pWdata->pWork;
    pWdata->AudReport     = 0;

    if (pWdata->Save == 0)   return;
    if (pWork->CallNum > 0)  return;

    GetInputVars_mGPS(pWdata);

    if (pInput->clGPSolve == Answer_No)        return;

    Solve_mGPS(pWdata);

    pWork->CallNum        ++;
    pWdata->AudReport     = 1;
    pWdata->Save          = 1;
    pWdata->zipSave       = 0;
    pWdata->scenClear     = 0;

    return;
    }

// Solve for GP factor _____________________________________________________________________________

void Solve_mGPS(WDATA_MGPS *pWdata)
    {
    int                      i;
    double                   P2;
    INPUT_MGPS              *pInput;
    INPUT_HANDLE_MGPS       *pInputH;
    WORK_MGPS               *pWork;
    RESULT_MGPS             *pResult;

    pInput                   = pWdata->pInput;
    pWork                    = pWdata->pWork;
    pResult                  = pWdata->pResult;
    pInputH                  = &pWdata->sInputH;

    pWork->hclPVFPtaxProfit  = sys_GetItemHandle(MLIAB, "clPVFPtaxProfit", IDX_TH, RET_REAL, BOOKS_NONE, BASIS_NONE, "");
    pWork->hclPVFAtaxProfit  = sys_GetItemHandle(MLIAB, "clPVFAtaxProfit", IDX_TH, RET_REAL, BOOKS_NONE, BASIS_NONE, "");
    pWork->hclPVFDistProfit  = sys_GetItemHandle(MLIAB, "clPVFDistProfit", IDX_TH, RET_REAL, BOOKS_NONE, BASIS_NONE, "");
    pWork->hclPVFCashPrem    = sys_GetItemHandle(MLIAB, "clPVFCashPrem",   IDX_TH, RET_REAL, BOOKS_NONE, BASIS_NONE, "");

    BeginLiabFutureProjection(1, 1);

    AllocTempCopyInput(pInputH->clGPSolveState);
    AllocTempCopyInput(pInputH->clGPSolveTGP);
    SetValueI(pInputH->clGPSolveState, Answer_Yes, 0, 0);

    P2 = 1.0 ;

    for (i = 1; i <= 10; i++)
    {
    pWork->P0                = P2;
    pWork->F0                = SolveA_mGPS(pWdata, pWork->P0);

    if (fabs(pWork->F0) <= .01)     break;

    pWork->P1                = pWork->P0 * 1.01 ;
    pWork->F1                = SolveA_mGPS(pWdata, pWork->P1);
    pWork->Fder              = (pWork->F1 - pWork->F0) / (pWork->P1 - pWork->P0);

    if (pWork->Fder != 0)        P2 = pWork->P1 - pWork->F1 / pWork->Fder;

    char dbuf[2000];

//  sprintf(dbuf, "P0 = %f \n F0 = %f \n P1 = %f \n F1 = %f \n Fder = %f \n P2 = %f \n",____________
//          pWork->P0, pWork->F0, pWork->P1, pWork->F1, pWork->Fder, P2) ;__________________________
//  MessageBox(0, dbuf, "Test", MB_OK) ;____________________________________________________________
    }

    EndLiabFutureProjection();

    FreeTempCopyInput(pInputH->clGPSolveState);
    FreeTempCopyInput(pInputH->clGPSolveTGP);

    pResult->clGPSolveFGP = P2;

    return;
    }

// Solve for GP factor _____________________________________________________________________________

double SolveA_mGPS(WDATA_MGPS *pWdata, double x)
    {
    double  Fx;
    INPUT_HANDLE_MGPS       *pInputH;
    INPUT_MGPS              *pInput;
    WORK_MGPS               *pWork;

    pInputH               = &pWdata->sInputH;
    pInput                = pWdata->pInput;
    pWork                 = pWdata->pWork;

    ResetLiabFutureProjection(1, 1);
    SetValueR(pInputH->clGPSolveTGP, x, 0, 0);

    if (pInput->clProfitType == ProfitType_PreTax)
        {
        Fx                    = GetValueR(pWork->hclPVFPtaxProfit, 1, 1)
                              - GetValueR(pWork->hclPVFCashPrem, 1, 1) * pInput->clGPSolveTarget;
        }

    if (pInput->clProfitType == ProfitType_AfterTax)
        {
        Fx                    = GetValueR(pWork->hclPVFAtaxProfit, 1, 1)
                              - GetValueR(pWork->hclPVFCashPrem, 1, 1) * pInput->clGPSolveTarget;
        }

    if (pInput->clProfitType == ProfitType_Distributable)
        {
        Fx                    = GetValueR(pWork->hclPVFDistProfit, 1, 1)
                              - GetValueR(pWork->hclPVFCashPrem, 1, 1) * pInput->clGPSolveTarget;
        }

    return Fx;
    }  