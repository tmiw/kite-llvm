method fib(n)
[
    n = n|int;
    decide 
    [
        (n == 0) [ 0; ],
        (n == 1) [ 1; ],
        true [ (fib(n - 1))|int + (fib(n - 2))|int; ]
    ];
];

i = 0;
until (i > 10)
[
    (fib(i))|print;
    i = i + 1;
];
