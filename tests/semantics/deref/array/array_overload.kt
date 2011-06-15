class X [
    construct() [ 1; ];
    method __op_deref_array__(index)
    [
        index;
    ];
];

(make X())[31337]|print;