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

/[Output methods for the Kite documentation system.]/

import "System.file";

class base_outputter
    /[Base outputter class.]/
[
    method new_document(
        filename /[Document's filename.]/,
        title /[Title of document.]/
    ) /[Create new document.]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ]
];

class html_outputter from base_outputter
    /[Output HTML documents to file.]/
[
    method new_document(
        filename /[Document's filename.]/,
        title /[Title of document.]/
    ) /[Create new document.]/
    [
        (make System.doc.outputters.html_document(filename, title));
    ]
];

class base_document
    /[Base document class.]/
[
    method close()
        /[Close document.]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ],
    
    method file_name(
        name /[Name of document]/
    ) /[Return actual file name of document]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ],
    
    method section(
        name /[Name of section]/
    ) /[Output section to file.]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ],
    
    method link(
        name /[Name of section]/,
        location /[Location to link to]/
    ) /[Output link to given location.]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ],
    
    method subsection(
        name /[Name of subsection]/
    ) /[Output subsection to file.]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ],
    
    method subsubsection(
        name /[Name of sub-sub-section]/
    ) /[Output sub-sub-section to file.]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ],
    
    method begin_list()
        /[Begin list of items.]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ],
    
    method end_list()
        /[End list of items.]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ],
    
    method begin_list_entry(
        name /[Name of list entry.]/,
        description /[Description of list entry.]/
    ) /[Begin list entry.]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ],
    
    method end_list_entry()
        /[End list entry.]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ],
    
    method sub_list_entry(
        name /[Name of sublist entry.]/,
        description /[Description of sublist entry.]/
    ) /[Begin sublist entry.]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ],
    
    method paragraph(
        text /[Text for paragraph.]/
    ) /[Output paragraph of text.]/
    [
        (make System.exceptions.NotImplemented())|throw;
    ]
];

class html_document from base_document
    /[HTML document class.]/
[
    property filehandle /[HTML document's file object.]/,
    global property header /[HTML header to include on generated documentation]/,
    global property footer /[HTML footer to include on generated documentation]/,
    
    this.header = "
<html>
<head>
<title>%s</title>
</head>
<body>
<h1>%s</h1>
<hr noshade=\"noshade\" />

",

    this.footer = "
</body>
</html>
",

    construct(
        filename /[File to create (excluding .html)]/,
        title /[Title of document.]/
    )
        /[Create new HTML document.]/
    [
        this.filehandle = make System.file(filename + ".html", "w");
        this.filehandle|write(
            this.header|format([title, title])
        );
    ],
    
    method close()
        /[Close document.]/
    [
        this.filehandle|write(
            this.footer
        );
        this.filehandle|close;
    ],
    
    method section(
        name /[Name of section]/
    ) /[Output section to file.]/
    [
        this.filehandle|write(
            "<h2>%s</h2>\n"|format([name])
        );
    ],
    
    method subsection(
        name /[Name of subsection]/
    ) /[Output subsection to file.]/
    [
        this.filehandle|write(
            "<h3>%s</h3>\n"|format([name])
        );
    ],
    
    method subsubsection(
        name /[Name of sub-sub-section]/
    ) /[Output sub-sub-section to file.]/
    [
        this.filehandle|write(
            "<h4>%s</h4>\n"|format([name])
        );
    ],
    
    method begin_list()
        /[Begin list of items.]/
    [
        this.filehandle|write(
            "<ul>\n"
        );
    ],
    
    method end_list()
        /[End list of items.]/
    [
        this.filehandle|write(
            "</ul>\n"
        );
    ],
    
    method begin_list_entry(
        name /[Name of list entry.]/,
        description /[Description of list entry.]/
    ) /[Begin list entry.]/
    [
        this.filehandle|write(
            "<li><code>%s</code>: %s<ul>\n"|format([name, description])
        );
    ],
    
    method end_list_entry() 
        /[End list entry.]/
    [
        this.filehandle|write(
            "</ul></li>\n"
        );
    ],
    
    method sub_list_entry(
        name /[Name of sublist entry.]/,
        description /[Description of sublist entry.]/
    ) /[Begin sublist entry.]/
    [
        this.filehandle|write(
            "<li><code>%s</code>: %s</li>\n"|format([name, description])
        );
    ],
    
    method paragraph(
        text /[Text for paragraph.]/
    ) /[Output paragraph of text.]/
    [
        this.filehandle|write(
            "<p>%s</p>\n"|format([text])
        );
    ],
    
    method link(
        name /[Name of section]/,
        location /[Location to link to]/
    ) /[Output link to given location.]/
    [
        "<a href=\"%s\">%s</a>"|format([location, name]);
    ],
    
    method file_name(
        name /[Name of document]/
    ) /[Return actual file name of document]/
    [
        (name + ".html");
    ]
];
