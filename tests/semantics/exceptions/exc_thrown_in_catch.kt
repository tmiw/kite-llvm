run [
    run [
        (make System.exceptions.InvalidArgument())|throw;
    ] catch [
        __exc|throw;
    ];
] catch [
    "Outer catch block"|print;
];