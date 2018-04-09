/*
* Copyright © 2018 Jesse Nicholson
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

using System;
using System.Runtime.InteropServices;

namespace NsfwNET
{
    /// <summary>
    /// The type of classifier you are creating. This must be accurate.
    /// </summary>
    public enum ClassifierType
    {
        /// <summary>
        /// This tells the underlying classification system that you're loading the NSFW Resnet 50
        /// model from OpenNSFW.
        /// </summary>
        Resnet = 0,

        /// <summary>
        /// This tells the underlying classification system that you're loading the NSFW Squeezenet model. 
        /// </summary>
        Squeezenet = 1,
    }

    /// <summary>
    /// The ImageClassifier class accepts full image binary payloads as the single parameter to a
    /// classify function, which will return a boolean value indicating whether or not the supplied
    /// image contains pornographic subject matter.
    /// </summary>
    /// <remarks>
    /// This class uses PInvoke to create and call an unmanaged native instance of a classifier. As
    /// such, this class implements the IDisposable interface to free unamanaged resources. Use it accordingly.
    /// </remarks>
    public class ImageClassifier : IDisposable
    {
        private IntPtr m_nativeObj = IntPtr.Zero;

        // Because we doubt sincerely the thread safety of openCV's dnn library.
        private object m_classLock = new object();

        /// <summary>
        /// Gets whether or not the classifier is empty. 
        /// </summary>
        /// <remarks>
        /// If this is true, then in most cases this is surely an indication that the model was not
        /// initialized correctly.
        /// </remarks>
        public bool IsEmpty
        {
            get
            {
                return ImageClassifierPInvoke.classifier_get_is_empty(m_nativeObj);
            }
        }

        /// <summary>
        /// Gets or sets the cutoff. Images who's score is above the set cutoff will be classified as
        /// pornographic. The default value is set based on experimentation.
        /// </summary>
        public double Cutoff
        {
            get
            {
                return ImageClassifierPInvoke.classifier_get_cutoff(m_nativeObj);
            }

            set
            {
                ImageClassifierPInvoke.classifier_set_cutoff(m_nativeObj, value);
            }
        }

        /// <summary>
        /// Constructs a new image classifier instance from the given model files.
        /// </summary>
        /// <param name="protoTextPath">
        /// Path to the prototxt file. 
        /// </param>
        /// <param name="caffeModelPath">
        /// Path to the model bin file. 
        /// </param>
        /// <param name="type">
        /// The type of classifier to create.
        /// </param>
        /// <param name="mean">
        /// The optional mean value for the supplied model. Internally, defaults to the predefined
        /// mean for the built in models (resnet 50, squeezenet).
        /// </param>
        public ImageClassifier(string protoTextPath, string caffeModelPath, ClassifierType type, double[] mean = null)
        {
            IntPtr meanPtr = IntPtr.Zero;

            if(mean != null)
            {
                switch(mean.Length)
                {
                    case 3:
                    case 4:
                        {

                        }
                        break;

                    default:
                        {
                            throw new ArgumentException("Mean value, if supplied, must have a length of 3 or 4. BGR(A).");
                        }
                }

                meanPtr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(double)) * mean.Length);
                Marshal.Copy(mean, 0, meanPtr, mean.Length);
            }

            m_nativeObj = ImageClassifierPInvoke.classifier_create_from_fs(protoTextPath, protoTextPath.Length, caffeModelPath, caffeModelPath.Length, (byte)type, meanPtr);

            if(meanPtr != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(meanPtr);
            }

            if(m_nativeObj == IntPtr.Zero)
            {
                throw new Exception("Failed to allocate native classifier.");
            }
        }

        /// <summary>
        /// Constructs a new image classifier instance from the caffe model in memory.
        /// </summary>
        /// <param name="prototxt">
        /// The prototxt data.
        /// </param>
        /// <param name="model">
        /// The model data.
        /// </param>
        /// <param name="type">
        /// The type of classifier to create.
        /// </param>
        /// <param name="mean">
        /// The optional mean value for the supplied model. Internally, defaults to the predefined
        /// mean for the built in models (resnet 50, squeezenet).
        /// </param>
        public ImageClassifier(byte[] prototxt, byte[] model, ClassifierType type, double[] mean = null)
        {
            IntPtr meanPtr = IntPtr.Zero;

            if(mean != null)
            {
                switch(mean.Length)
                {
                    case 3:
                    case 4:
                        {

                        }
                        break;

                    default:
                        {
                            throw new ArgumentException("Mean value, if supplied, must have a length of 3 or 4. BGR(A).");
                        }
                }

                meanPtr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(double)) * mean.Length);
                Marshal.Copy(mean, 0, meanPtr, mean.Length);
            }

            m_nativeObj = ImageClassifierPInvoke.classifier_create_from_memory(prototxt, prototxt.Length, model, model.Length, (byte)type, meanPtr);

            if(meanPtr != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(meanPtr);
            }

            if(m_nativeObj == IntPtr.Zero)
            {
                throw new Exception("Failed to allocate native classifier.");
            }
        }

        /// <summary>
        /// Classifies the image input. Note that you need to ensure that you are supplying an image
        /// in JPEG encoding.
        /// </summary>
        /// <param name="imageData">
        /// The image data to classify. 
        /// </param>
        /// <returns>
        /// True of the supplied image was classified as pornography, false otherwise. 
        /// </returns>
        public bool ClassifyImage(byte[] imageData)
        {
            bool result = false;

            lock(m_classLock)
            {
                result = ImageClassifierPInvoke.classifier_classify(m_nativeObj, imageData, imageData.Length);
            }

            return result;
        }

        /// <summary>
        /// Gets the probability of the input image being positive.
        /// in JPEG encoding.
        /// </summary>
        /// <param name="imageData">
        /// The image data to classify. 
        /// </param>
        /// <returns>
        /// The probability that the input is a positive match.
        /// </returns>
        public double GetPositiveProbability(byte[] imageData)
        {
            double result = 0d;

            lock(m_classLock)
            {
                result = ImageClassifierPInvoke.classifier_get_positive_probability(m_nativeObj, imageData, imageData.Length);
            }

            return result;
        }

        #region IDisposable Support

        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if(!disposedValue)
            {
                if(disposing)
                {
                    // TODO: dispose managed state (managed objects).
                }

                if(m_nativeObj != IntPtr.Zero)
                {
                    ImageClassifierPInvoke.classifier_destroy(m_nativeObj);
                    m_nativeObj = IntPtr.Zero;
                }
                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free
        //       unmanaged resources. ~ImageClassifier() { // Do not change this code. Put cleanup
        // code in Dispose(bool disposing) above. Dispose(false); }

        // This code added to correctly implement the disposable pattern.
        public void Dispose()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
            Dispose(true);
            // TODO: uncomment the following line if the finalizer is overridden above. GC.SuppressFinalize(this);
        }

#endregion IDisposable Support
    }
}