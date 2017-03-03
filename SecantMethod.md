# Secant Method

**Initialization : P2 = 1**

**1. P0 = P2**

**2. F0 = SolveA_mGPS(P0)**

>2.1. Multiply Factor = P0  
>2.2. Future Projection  
>2.3. Load PVF Values  
>2.4. Fx = PVFClaim - PVFIncome  
>2.5. return Fx  
>2.6. Calculate Sum of Fx for all in-forces by scenarios  
>2.7. Sort Fx  
>2.8. Calculate CTE70  
>2.9. F0 = CTE70  

**3. Tolerance check (F0 <= 0.01)**

**4. P1 = P0 * 1.01**

**5. F1 = SolveA_mGPS(P1)**

>5.1. Multiply Factor = P1  
>5.2. Future Projection  
>5.3. Load PVF Values  
>5.4. Fx = PVFClaim - PVFIncome  
>5.5. return Fx  
>5.6. Calculate Sum of Fx for all in-forces by scenarios  
>5.7. Sort Fx  
>5.8. Calculate CTE70  
>5.9. F1 = CTE70  

**6. Fder = (F1 - F0) / (P1 - P0)**

**7. Fder <> 0**

>7.1. P2 = P1 - F1 / Fder  

**8. 1 로 가서 반복(Iteration 횟수 설정해야 함)**
