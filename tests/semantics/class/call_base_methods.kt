class X [
    construct() [ this.val = 1; ];
    method get_val() [ this.val; ];
];

class Y from X [
    construct() [ base|__init__(); ];
    method get_val() [ (base|get_val + 1); ];
];

v = make Y();
v|get_val|print;
