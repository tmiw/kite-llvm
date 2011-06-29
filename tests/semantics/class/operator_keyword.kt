class X [
    construct() [
        this.x = 1;
    ];
    
    operator plus(y) [
        (this.x + y);
    ];
];


z = make X();
(z + 1)|print;