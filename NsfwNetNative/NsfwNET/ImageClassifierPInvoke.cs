/*
* Copyright © 2018 Jesse Nicholson
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_create_from_fs", CallingConvention = CallingConvention.Cdecl)]
        public static extern System.IntPtr classifier_create_from_fs([In()] [MarshalAs(UnmanagedType.LPStr)] string protoTxtPath, int protoTxtLen, [In()] [MarshalAs(UnmanagedType.LPStr)] string caffeModelPath, int caffeModelPathLen, byte classifierType, IntPtr mean);

        /// Return Type: PVOID->void*
        ///protoTxtPath: char*
        ///protoTxtLen: uint32_t->int
        ///caffeModelPath: char*
        ///caffeModelPathLen: uint32_t->int
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_create_from_memory", CallingConvention = CallingConvention.Cdecl)]
        public static extern System.IntPtr classifier_create_from_memory([In()] byte[] protoTxtData, int protoTxtDataLen, [In()] byte[] caffeModelData, int caffeModelDataLen, byte classifierType, IntPtr mean);

        /// Return Type: boolean
        ///classifier: PVOID->void*
        ///imageData: unsigned char*
        ///imageDataLength: uint32_t->int
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_classify", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool classifier_classify(System.IntPtr classifier, byte[] imageData, int imageDataLength);

        /// Return Type: boolean
        ///classifier: PVOID->void*
        ///imageData: unsigned char*
        ///imageDataLength: uint32_t->int
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_get_positive_probability", CallingConvention = CallingConvention.Cdecl)]        
        public static extern double classifier_get_positive_probability(System.IntPtr classifier, byte[] imageData, int imageDataLength);

        /// Return Type: boolean
        ///classifier: PVOID->void*
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_get_is_empty", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool classifier_get_is_empty(System.IntPtr classifier);

        /// Return Type: double
        ///classifier: PVOID->void*
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_get_cutoff", CallingConvention = CallingConvention.Cdecl)]
        public static extern double classifier_get_cutoff(System.IntPtr classifier);

        /// Return Type: void
        ///classifier: PVOID->void*
        ///cutoff: double
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_set_cutoff", CallingConvention = CallingConvention.Cdecl)]
        public static extern void classifier_set_cutoff(System.IntPtr classifier, double cutoff);

        /// Return Type: void
        ///classifier: PVOID->void*
        [DllImport("NsfwNetNative.dll", EntryPoint = "classifier_destroy", CallingConvention = CallingConvention.Cdecl)]
        public static extern void classifier_destroy(System.IntPtr classifier);
    }
}