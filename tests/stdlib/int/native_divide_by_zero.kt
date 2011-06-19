run [
    (6 / 0)|print;
] catch [
    __exc.__name|print;
    __exc.message|print;
];
