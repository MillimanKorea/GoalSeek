# Secant Method

**Initialization : P0 = 1**

**1. P0 = P2**

**2. F0 = SolveA_mGPS(P0)**

>2.1. Multiply Factor = P0  
>2.2. Future Projection  
>2.3. Load PVF Values  
>2.4. Fx = PVFClaim00 - PVFIncome00 * P0  
>2.5. return Fx

**3. Tolerance check (F0 <= 0.01)**

**4. P1 = P0 * 1.01**

**5. F1 = SolveA_mGPS(P1)**

>5.1. Multiply Factor = P1  
>5.2. Future Projection  
>5.3. Load PVF Values  
>5.4. Fx = PVFClaim01 - PVFIncome01 * P1  
>5.5. return Fx

**6. Fder = (F1 - F0) / (P1 - P0)**

> 6.1. Fder = (PVFClaim00 - PVFIncome00 - PVFClaim01 + PVFIncome01 * 1.01) / 0.01

**7. Fder <> 0**

>7.1. P2 = P1 - F1 / Fder  
>>7.1.1. P2 = 1.01 - (PVFClaim01 - PVFIncome01 * 1.01) * 0.01 / (PVFClaim00 - PVFIncome00 - PVFClaim01 + PVFIncome01 * 1.01)

**8. 1 로 가서 반복(Iteration 횟수 설정해야 함)**
