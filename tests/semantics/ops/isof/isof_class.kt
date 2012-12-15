class X [
    construct() [ 1; ]
];

class Y from X [
    construct() [ 1; ]
];

class Z [
    construct() [ 1; ]
];

v = make Y();
(v isof Y)|print;
(v isof X)|print;
(v isof Z)|print;
