run [
    (5.0|obj % 3|obj)|print;
] catch [
    __exc.__name|print;
    __exc.message|print;
];
