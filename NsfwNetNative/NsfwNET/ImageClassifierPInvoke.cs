/*
* Copyright © 2017 Jesse Nicholson
* Copyright © 2016, Yahoo Inc.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions
* and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of
* conditions and the following disclaimer in the documentation and/or
* other materials provided with
* the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
* THE COPYRIGHT HOLDER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER
* IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*/

using System;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;

namespace NsfwNET
{
    internal class ImageClassifierPInvoke
    {
        static ImageClassifierPInvoke()
        {
            // Add diff arch native lib folders to path environment so that
            // .NET will properly load the right one for Pinvoke.

            // Modify PATH var to include our WinDivert DLL's so that the LoadLibrary function
            // will find whatever WinDivert dll required for the current architecture.
            var path = new[] { Environment.GetEnvironmentVariable("PATH") ?? string.Empty };

            var dllSearchPaths = new[]
            {
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "x86"),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "x64"),
            };

            string newPath = string.Join(Path.PathSeparator.ToString(), path.Concat(dllSearchPaths));

            Environment.SetEnvironmentVariable("PATH", newPath);
        }

        /// Return Type: PVOID->void*
        ///protoTxtPath: char*
        ///protoTxtLen: uint32_t->int
        ///caffeModelPath: char*
        ///caffeModelPathLen: uint32_t->int
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_create")]
        public static extern System.IntPtr classifier_create([In()] [MarshalAs(UnmanagedType.LPStr)] string protoTxtPath, int protoTxtLen, [In()] [MarshalAs(UnmanagedType.LPStr)] string caffeModelPath, int caffeModelPathLen);

        /// Return Type: boolean
        ///classifier: PVOID->void*
        ///imageData: unsigned char*
        ///imageDataLength: uint32_t->int
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_classify")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool classifier_classify(System.IntPtr classifier, byte[] imageData, int imageDataLength);

        /// Return Type: boolean
        ///classifier: PVOID->void*
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_get_is_empty")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool classifier_get_is_empty(System.IntPtr classifier);

        /// Return Type: double
        ///classifier: PVOID->void*
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_get_cutoff")]
        public static extern double classifier_get_cutoff(System.IntPtr classifier);

        /// Return Type: void
        ///classifier: PVOID->void*
        ///cutoff: double
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_set_cutoff")]
        public static extern void classifier_set_cutoff(System.IntPtr classifier, double cutoff);

        /// Return Type: void
        ///classifier: PVOID->void*
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_destroy")]
        public static extern void classifier_destroy(System.IntPtr classifier);
    }
}