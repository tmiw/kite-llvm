run [
    (6.0 / 0.0)|print;
] catch [
    __exc.__name|print;
    __exc.message|print;
];
