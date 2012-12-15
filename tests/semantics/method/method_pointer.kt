class X [
    method __op_reduce__(fxn)
    [
        fxn(this.i|obj);
    ],
  
    construct() [ this.i = 1; ]
];

method f(i) [ (i + 1); ];

((make X())<|f__oo)|print;

