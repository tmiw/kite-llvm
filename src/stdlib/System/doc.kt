##############################################################################
# Copyright (c) 2008, Mooneer Salem
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Kite Language organization nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY MOONEER SALEM ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL MOONEER SALEM BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##############################################################################

/[The Kite documentation system.]/

import "System.exceptions.NotImplemented";
import "System.doc.outputters";

class doc_node
     /[Common interface for documentation nodes.]/
[
    property name /[Full name of current node]/,
    property children /[Child nodes of the current node.]/,
    property description /[Description of current node.]/,

    construct(
        name /[Name of node.]/
    ) /[Construct new document node object.]/
    [
        this.children = [];
        this.name = name;
    ],
    
    method output(
        formatter /[The System.doc.outputters object to use.]/
    ) /[Outputs documentation for the given node.]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ],

    method add(
        child /[Child node to add.]/
    ) /[Add node to current node's list of children.]/
    [
        this.children << child;
    ]
];

class method_argument_node from System.doc.doc_node
    /[Document node corresponding to a method argument.]/
[
    construct(
        name /[Name of method argument.]/,
        description /[Description of method argument.]/
    ) /[Construct new method argument node.]/
    [
        base|__init__(name);
        this.description = description;
    ]
];

class method_node from System.doc.doc_node
    /[Document node corresponding to a method.]/
[
    construct(
        name /[Name of method.]/,
        description /[Description of method.]/
    ) /[Construct new method node.]/
    [
        base|__init__(name);
        this.description = description;
    ],
    
    method output(
        formatter /[The System.doc.outputters object to use.]/
    ) /[Outputs documentation for the given method.]/
    [   
        names = [];
        descrs = [];
        name_str = this.name + "(";
        
        i = 0;
        while (i < this.children|count) [
            child = this.children[i];
            names|append(child.name);
            descrs|append(child.description);
            name_str = name_str + child.name;
            i = i + 1;
            decide [ (i < this.children|count) [ name_str = name_str + ", "; ] ];
        ];
        name_str = name_str + ")";

        formatter|begin_list_entry(name_str, this.description);
        i = 0;
        while (i < names|count) [
            formatter|sub_list_entry(names[i], descrs[i]);
            i = i + 1;
        ];
        formatter|end_list_entry;
    ]
];

class property_node from System.doc.doc_node
    /[Document node corresponding to a property.]/
[
    construct(
        name /[Name of property.]/,
        description /[Description of property.]/
    ) /[Construct new method node.]/
    [
        base|__init__(name);
        this.description = description;
    ],
    
    method output(
        formatter /[The System.doc.outputters object to use.]/
    ) /[Outputs documentation for the given property.]/
    [
        formatter|begin_list_entry(this.name, this.description);
        formatter|end_list_entry;
    ]
];

class constructor_node from System.doc.method_node
    /[Document node corresponding to a constructor.]/
[
    construct(
        name /[Name of class.]/,
        description /[Description of constructor.]/
    ) /[Construct new constructor node.]/
    [
        base|__init__("make " + name, description);
    ]
];

class destructor_node from System.doc.method_node
    /[Document node corresponding to a destructor.]/
[
    construct(
        name /[Class name.]/,
        description /[Description of destructor.]/
    ) /[Construct new destructor node.]/
    [
        base|__init__("destruct " + name);
        this.description = description;
    ]
];

class operator_node from System.doc.method_node
    /[Document node corresponding to an operator.]/
[
    construct(
        name /[Name of operator.]/,
        description /[Description of operator.]/
    ) /[Construct new operator node.]/
    [
        base|__init__("operator " + name);
        this.description = description;
    ]
];

class class_node from System.doc.doc_node 
    /[Documentation node corresponding to a class.]/
[
    property inherits /[Name of inherited class]/,

    construct(
        name /[Full name of class]/,
        inherits /[Inherited class name]/
    ) /[Construct new class node.]/
    [
        base|__init__(name);
        this.inherits = inherits;
    ],
    
    method output(
        formatter /[The System.doc.outputters object to use.]/
    ) /[Outputs documentation for the given class.]/
    [
        doc = formatter|new_document(name, "Class documentation: " + name);
        
        doc|paragraph(description);
        
        doc|section("Inherits");
        decide [
            (this.inherits is System.null) [
                doc|paragraph("Nothing");
            ],
            true [
                doc|paragraph(doc|link(this.inherits, doc|file_name(this.inherits)));
            ]
        ];
        
        doc|section("Classes contained");
        doc|begin_list;
        i = 0;
        while (i < this.children|count) [
            child = this.children[i];
            decide [
                (child is System.doc.class_node) [
                    doc|begin_list_entry(
                        doc|link(child.name, doc|file_name(child.name)),
                        child.description
                    );
                    doc|end_list_entry;
                    child|output(formatter);
                ]
            ];
            i = i + 1;
        ];
        doc|end_list;
        
        doc|section("Constructors");
        doc|begin_list;
        i = 0;
        while (i < this.children|count) [
            child = this.children[i];
            decide [
                (child is System.doc.constructor_node) [
                    child|output(doc);
                ]
            ];
            i = i + 1;
        ];
        doc|end_list;
        
        doc|section("Destructor");
        doc|begin_list;
        i = 0;
        while (i < this.children|count) [
            child = this.children[i];
            decide [
                (child is System.doc.destructor_node) [
                    child|output(doc);
                ]
            ];
            i = i + 1;
        ];
        doc|end_list;
        
        doc|section("Operators");
        doc|begin_list;
        i = 0;
        while (i < this.children|count) [
            child = this.children[i];
            decide [
                (child is System.doc.operator_node) [
                    child|output(doc);
                ]
            ];
            i = i + 1;
        ];
        doc|end_list;
        
        doc|section("Properties");
        doc|begin_list;
        i = 0;
        while (i < this.children|count) [
            child = this.children[i];
            decide [
                (child is System.doc.property_node) [
                    child|output(doc);
                ]
            ];
            i = i + 1;
        ];
        doc|end_list;
        
        doc|section("Methods");
        doc|begin_list;
        i = 0;
        while (i < this.children|count) [
            child = this.children[i];
            decide [
                (child is System.doc.method_node) [
                    child|output(doc);
                ]
            ];
            i = i + 1;
        ];
        doc|end_list;
        
        doc|close;
    ]
];

##############################################################################
# Public interface to kdoc.
##############################################################################
import "System.collections";

method generate_property(
    cls /[Class object property belongs to.]/,
    name /[Property name.]/
) /[Generate documentation for a property.]/ 
[
    make System.doc.property_node(name, cls|get_property_string(name));
];

method generate_method_arguments(
    method_doc /[System.doc.method_node object.]/,
    method_obj /[System.method object corresponding to the method to generate arguments for.]/
) /[Generate documentation for method arguments.]/
[
    args = method_obj|get_param_names;
    i = 0;
    while (i < args|count) [
        method_doc|add(
            make System.doc.method_argument_node(
                args[i], 
                method_obj|get_param_doc(args[i])
            )
        );
        i = i + 1;
    ];
];

method generate_constructor(
    class_name /[Name of class.]/,
    method_obj /[System.method object corresponding to constructor.]/
) /[Generate documentation for a constructor.]/
[
    ret = make System.doc.constructor_node(class_name, method_obj|doc);
    generate_method_arguments(ret, method_obj);
    ret;
];

method generate_method(
    method_name /[Name of method.]/,
    method_obj /[System.method object corresponding to a method.]/
) /[Generate documentation for a method.]/
[
    ret = make System.doc.method_node(method_name, method_obj|doc);
    generate_method_arguments(ret, method_obj);
    ret;
];

method generate_destructor(
    class_name /[Name of class.]/,
    method_obj /[System.method object corresponding to destructor.]/
) /[Generate documentation for a destructor.]/
[
    ret = make System.doc.destructor_node(class_name, method_obj|doc);
    ret;
];

method generate_operator(
    op_name /[Name of operator.]/,
    op_method /[System.method object corresponding to operator.]/
) /[Generate documentation for an operator.]/
[
    ret = make System.doc.operator_node(op_name, op_method|doc);
    generate_method_arguments(ret, op_method);
    ret;
];

method generate_doc_helper(
    cls /[Class object to generate documentation for.]/, 
    done_list /[System.collections.binary_tree object with list of generated classes.]/
) /[Helper method for generating documentation (not intended for public use)]/ 
[
    decide [
        (done_list[cls|type] is System.null) [
            decide [
                (cls|get_base_object is System.null) [
                    ret = make System.doc.class_node(cls|type, null);
                ],
                true [
                    ret = make System.doc.class_node(cls|type, cls|get_base_object|type);
                ]
            ];
            
            ret.description = cls|doc;
            
            done_list|set(cls|type, ret);
            
            # Properties first.
            tmp = cls|list_properties;
            i = 0;
            while (i < tmp|count) [
                j = cls|get_property(tmp[i]);
                decide [
                    (j|is_class) [
                        ret|add(generate_doc_helper(j, done_list));
                    ],
                    true [
                        ret|add(System.doc|generate_property(cls, tmp[i]));
                    ]
                ];
                i = i + 1;
            ];
            
            # Methods/constructors.
            tmp = cls|list_methods;
            i = 0;
            while (i < tmp|count) [
                j = 0;
                tmp_method = tmp[i + 1];
                while (j < tmp_method|count) [
                    decide [
                        (tmp[i] == "__init__") [
                            ret|add(System.doc|generate_constructor(cls|type, cls|get_method(tmp[i], tmp_method[j])));
                        ],
                        true [
                            ret|add(System.doc|generate_method(tmp[i], cls|get_method(tmp[i], tmp_method[j])));                        
                        ]
                    ];
                    j = j + 1;
                ];
                i = i + 2;
            ];
            
            # Destructor
            decide [
                (not (cls|get_destructor is System.null)) [
                    ret|add(System.doc|generate_destructor(cls|type, cls|get_destructor));
                ]
            ];
            
            # Operators
            i = 0;
            while (i < System.object.operators.number_of_operators) [
                decide [
                    (not (cls|get_operator_method(i) is System.null)) [
                        ret|add(System.doc|generate_operator(cls.operators|to_str(i), cls|get_operator_method(i)));
                    ]
                ];
                i = i + 1;
            ];
            
            ret;
        ],
        true [ done_list[cls|type]; ]
    ];
];

method generate_doc(
    cls /[Class object to generate documentation for.]/
) /[Generate documentation objects for the given class.]/ 
[
    tmp = make System.collections.binary_tree();
    generate_doc_helper(cls, tmp);
];
