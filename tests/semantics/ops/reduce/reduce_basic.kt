class X [
    construct(i) [ this.val = i; ];
    method __op_reduce__(fun)
    [
        this.val + 1;
    ];
];

v = make X(31336);
(v<|fxn)|print;