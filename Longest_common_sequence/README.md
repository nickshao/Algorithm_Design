# Longest Common Subsequences

This program aims to find **all** longest common subsequences of given two sequences.

+ <font size=4 face="黑体">**I/O format**</font>:
    + Each input contains two sequences, both of them satisfying   : 
     0 $\leq$ *length(A)*,*length(B)* $\leq$ 100.
    + This program will output `length of LCS` and `number of LCS` in the first line, and each of them listed in the following output.
+ <font size=4 face="黑体">**How to run**</font>:
    + Using Makefile:
```$make``` to generate executable file.  
```$make clean``` to remove existed executable file.  
    + compile command:
    ```$g++ -O2 -std=c++11 LCS.cpp -o LCS```

+ <font color=#cc0000 size=4 face="黑体">**Sample Input**</font>:
```./LCS QHCCDETMFPPWZGIFEFUA GZXPAWWOOCZEMBIPHECM```
+ <font color=#6600ff size=4 face="黑体">**Sample Output**</font>:
    ```
    5 10
    CEMIE
    CEMIE
    CEMPE
    CEMPE
    CEMPE
    CEMPE
    PWZIE
    PWZIE
    PWZIE
    PWZIE
    ```
+ <font color=#006600 size=4 face="黑体">**Reference**</font>:
    >http://www.csie.ntnu.edu.tw/~u91029/LongestCommonSubsequence.html
     >https://www.geeksforgeeks.org/longest-common-subsequence-dp-4/


