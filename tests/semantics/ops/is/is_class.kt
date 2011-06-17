class X [
    construct() [ 1; ];
];

class Y [
    construct() [ 1; ];
];

v = make X();
(v is X)|print;
(v is Y)|print;
