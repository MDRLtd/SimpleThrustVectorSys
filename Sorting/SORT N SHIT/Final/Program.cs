using System;
using System.IO;

namespace Translate
{
    class Program
    {
        static void Main(string[] args)
        {
            string dirname = "";
            string dir = @"E:\Sort1";
            string data = @"E:\infromat\Sort\FINAL\realdatacopy.TXT"; //paste the url to the data here
            string[] sign = {"!", "@", "#", "$", "%", "*", "?", "&"};
            string[] paths = {
                "Tijd.txt",
                "Temperatuur.txt",
                "Druk.txt",
                "HoogtePT1.txt",
                "Hoogte.txt",
                "VersnellingALL.txt",
                "GyroscoopALL.txt",
                "MagnetometerALL.txt"
            };

            char[] triminbois =  {'m', 's', ';', 'C', 'P', 'a', ' ', '*'};

            int i = 0;

            try{
                if(Directory.Exists(dir)){

                    if(Directory.GetFiles(dir) != null){

                        Console.Write("Directory at {0} isnt empty. Want to create a clear Directory? (y/anything other than y) ", dir);

                        if(Console.ReadLine() == "y"){

                            Console.Write("Name the new directory: ");
                            dirname = Console.ReadLine();

                            if(dirname == ""){

                                Console.WriteLine("The files won't be created in a folder. Are you sure you want to proceed? (y/anything other than y) ");
                                string a = Console.ReadLine();

                                if(a == "y"){

                                    Directory.CreateDirectory(@"E:\" + dirname);

                                } else {

                                    while(Directory.Exists(@"E:\" + dirname)){

                                        Console.Write("Invalid directory. Choose a different name: ");
                                        dirname = Console.ReadLine();
                                    }

                                    if(!Directory.Exists(@"E:\" + dirname)){

                                        Directory.CreateDirectory( @"E:\" + dirname);
                                        Console.WriteLine(@"Created new directory at E:\{0} press any key to continue", dirname);
                                    }
                                }
                            } else{
                                while(Directory.Exists(@"E:\" + dirname)){
                                    Console.Write("Invalid directory. Choose a different name: ");
                                    dirname = Console.ReadLine();
                                }
                                if(!Directory.Exists(@"E:\" + dirname)){
                                    Directory.CreateDirectory(@"E:\" + dirname);
                                    Console.WriteLine(@"Created new directory at E:\{0} press any key to continue", dirname);
                                }
                            }

                        } else {

                            Console.Write("start fresh? (y/anything other than y) ");
                            string ans = Console.ReadLine();

                            if(ans == "y"){

                                Console.WriteLine("oki");

                                foreach(string p in paths){
                                    if(File.Exists(p)){
                                        Console.WriteLine("\tdeleting {0}", p);
                                        File.Delete(p);
                                    }
                                }
                            } else Console.WriteLine("mkay");
                        }
                    }
                } else {
                    Directory.CreateDirectory(dir);
                }
            } catch (Exception e){
                Console.WriteLine(e.ToString());
            }

            Console.ReadLine();
            Console.WriteLine("Press enter to begin the sorting process");

            try{

                using(StreamReader sr = File.OpenText(data)){

                    string s = "";

                    s = sr.ReadLine();

                    while(s != null){

                        for(i = 0; i < sign.Length; i++){

                            if(s.Contains(sign[i])){

                                int index = 0;
                                index = s.IndexOf(sign[i]);
                                string towriteto = @"E:\" + dirname + @"\" + paths[i];
                                using(StreamWriter sw = File.AppendText(towriteto)){
                                    Console.WriteLine("\t{0}", s.Substring(index + 2).Trim(triminbois));
                                    sw.WriteLine(s.Substring(index + 2).Trim(triminbois));
                                }   
                            }
                        }
                    s = sr.ReadLine();
                    }
                }
            }  
            catch(Exception e){
                Console.WriteLine(e.ToString());
            }
            Console.WriteLine("\n\n|~~~~ \t D\tO\tN\tE\t ~~~~|" );
            Console.ReadLine();
        } 
    }
}