class X [
    construct() [ this.i = 5; ],
    method y() [ i|print; ]
];

v = make X();
v|y;
