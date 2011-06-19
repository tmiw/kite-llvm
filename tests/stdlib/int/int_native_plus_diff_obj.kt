run [
    (1 + 3.0|obj)|print;
] catch [
    __exc.__name|print;
    __exc.message|print;
];