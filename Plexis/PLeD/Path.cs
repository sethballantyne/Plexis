// Copyright(c) 2018 Seth Ballantyne <seth.ballantyne@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files(the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PLeD
{
    /// <summary>
    /// Represents a single resource path read from paths.xml
    /// </summary>
    struct Path
    {
        /// <summary>
        /// <b>true</b> if all the subdirectories within the directory pointed to be ResourcePath
        /// should also be processed for the same resource type contained in ResourcePath, otherwise <b>false</b>.
        /// This is the value of the inclueSubDirectories attribute contained within every &lt;path&gt element.
        /// </summary>
        public bool IncludeSubDirectories;

        /// <summary>
        /// The value of a &lt;path&gt; element in paths.xml; points to directory containg a resource of a particular type. 
        /// </summary>
        public string ResourcePath;
    }
}
