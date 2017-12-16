using NsfwNET;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ClassifierTest
{
    class Program
    {
        static void Main(string[] args)
        {
            string protoTxtPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "deploy.prototxt");
            string mdlBinPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "resnet_50_1by2_nsfw.caffemodel");

            var classifier = new ImageClassifier(protoTxtPath, mdlBinPath);

            Console.WriteLine("Loaded classifier.");
            Console.WriteLine("Classifier has cutoff of {0}.", classifier.Cutoff);

            string badImagesTesting = @"J:\bad_stuff\test_bad_scaled";
            string goodImagesTesting = @"J:\bad_stuff\test_good_scaled";

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
                    var outPath = Path.Combine(@"J:\garbage", Path.GetFileName(img));
                    File.Move(img, outPath);
                    ++badWrong;
                }

                if((badRight + badWrong) % 10 == 0)
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

                if((goodWrong + goodRight) % 10 == 0)
                {
                    Console.WriteLine("Classified {0} of {1} good images.", goodWrong + goodRight, min);
                }
            }

            sw.Stop();

            Console.WriteLine("Classified pornographic images with an accuracy of {0}%.", 100d * ((double)badRight / (double)(badRight + badWrong)));

            Console.WriteLine("Classified non-pornographic images with an accuracy of {0}%.", 100d * ((double)goodRight / (double)(goodRight + goodWrong)));

            Console.WriteLine("Took an average of {0} msec per image to classify.", sw.ElapsedMilliseconds / (double)(min * 2));

            Console.ReadLine();
        }
    }
}
