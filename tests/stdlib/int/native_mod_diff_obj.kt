run [
    (5 % 3.0|obj)|print;
] catch [
    __exc.__name|print;
    __exc.message|print;
];
