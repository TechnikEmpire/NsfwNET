/*
* Copyright © 2018 Jesse Nicholson
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

using NsfwNET;
using System;
using System.Diagnostics;
using System.IO;
using System.Linq;

namespace ClassifierTest
{
    internal unsafe class Program
    {
        public static ImageClassifier GetClassifier(ClassifierType type)
        {
            string protoTxtPath = string.Empty;
            string mdlBinPath = string.Empty;

            switch(type)
            {
                case ClassifierType.Resnet:
                    {
                        protoTxtPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "resnet_nsfw.prototxt");
                        mdlBinPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "resnet_50_1by2_nsfw.caffemodel");
                    }
                    break;

                case ClassifierType.Squeezenet:
                    {
                        protoTxtPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "squeezenet_nsfw.prototxt");
                        mdlBinPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "squeezenet_nsfw.caffemodel");
                    }
                    break;
            }

            var classifier = new ImageClassifier(protoTxtPath, mdlBinPath, type);

            if(classifier.IsEmpty)
            {
                throw new Exception("Classifier shouldn't be empty. Are you model paths correct?");
            }

            return classifier;
        }

        private static void TestClassifier(ImageClassifier classifier, ClassifierType type, bool verbose = false)
        {
            Console.WriteLine("{0} classifier has cutoff of {1}.", type, classifier.Cutoff);
            
            string badImagesTesting = @"SOME BUD STUFF BRO";
            string goodImagesTesting = @"SOME GOOD STUFF KIDDO";

            var badImgs = Directory.GetFiles(badImagesTesting).ToList();
            var goodImgs = Directory.GetFiles(goodImagesTesting).ToList();

            // Cut down our test base so it's not so huge.
            var min = Math.Min(badImgs.Count, goodImgs.Count);

            if(min > 1000)
            {
                min = 1000;
            }

            badImgs = badImgs.GetRange(0, min);
            goodImgs = goodImgs.GetRange(0, min);

            // Shuffle things up a bit.
            Random r = new Random();
            badImgs = badImgs.OrderBy(x => r.Next()).ToList();
            goodImgs = goodImgs.OrderBy(x => r.Next()).ToList();

            int goodRight = 0, goodWrong = 0, badRight = 0, badWrong = 0;

            var sw = new Stopwatch();
            sw.Start();

            foreach(var img in badImgs)
            {
                var imgData = File.ReadAllBytes(img);
                
                if(classifier.ClassifyImage(imgData))
                {
                    ++badRight;
                }
                else
                {
                    // If you feel like inspecting false negatives.
                    //var outPath = Path.Combine(SOME PLACE FOR FALSE NEGATIVES, Path.GetFileName(img));
                    //File.Move(img, outPath);
                    ++badWrong;
                }

                if(verbose && (badRight + badWrong) % 10 == 0)
                {
                    Console.WriteLine("Classified {0} of {1} bad images.", badRight + badWrong, min);
                }
            }

            foreach(var img in goodImgs)
            {
                var imgData = File.ReadAllBytes(img);

                if(classifier.ClassifyImage(imgData))
                {
                    ++goodWrong;
                }
                else
                {
                    ++goodRight;
                }

                if(verbose && (goodWrong + goodRight) % 10 == 0)
                {
                    Console.WriteLine("Classified {0} of {1} good images.", goodWrong + goodRight, min);
                }
            }

            sw.Stop();

            Console.WriteLine("Classifier with type {0} classified pornographic images with an accuracy of {1}%.", type, 100d * ((double)badRight / (double)(badRight + badWrong)));

            Console.WriteLine("Classifier with type {0} classified non-pornographic images with an accuracy of {1}%.", type, 100d * ((double)goodRight / (double)(goodRight + goodWrong)));

            Console.WriteLine("Classifier with type {0} took an average of {1} msec per image to classify.", type, sw.ElapsedMilliseconds / (double)(min * 2));
        }

        private static void Main(string[] args)
        {
            var sw = new Stopwatch();
            
            var squeezeClassifier = GetClassifier(ClassifierType.Squeezenet);

            sw.Start();
            TestClassifier(squeezeClassifier, ClassifierType.Squeezenet);
            sw.Stop();

            double firstTime = sw.ElapsedMilliseconds;
            sw.Reset();
            
            var resnetClassifier = GetClassifier(ClassifierType.Resnet);

            sw.Restart();
            TestClassifier(resnetClassifier, ClassifierType.Resnet);
            sw.Stop();

            double secondTime = sw.ElapsedMilliseconds;

            Console.WriteLine("SqueezeNet classifier was {0} times faster.", secondTime / firstTime);

            Console.ReadLine();
        }
    }
}