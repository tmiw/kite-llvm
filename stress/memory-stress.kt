class X [ construct(i) [ i|print; ]; destruct [ "DESTROY"|print; ]; ]; 
i = 0;
until (i >= 1000000) [
    y = make X(i);
    i = i + 1;
];
1;
