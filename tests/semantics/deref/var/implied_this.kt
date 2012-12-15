class X [
    construct() [ this.i = 5; ],
    method y() [ i; ]
];

v = make X();
v|y|print;
