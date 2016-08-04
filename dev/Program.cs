using System;
using System.IO;
using Api = Intuit.Spc.Map.Features.EntitlementClient.Api;
using Common = Intuit.Spc.Map.Features.EntitlementClient.Common;
using System.Reflection;

namespace offer
{
    class Program
    {
        static void Main(string[] args)
        {
            
            Intuit.Spc.Map.Features.EntitlementClient.Api.Entitlements.IEntitlementManager e = new Intuit.Spc.Map.Features.EntitlementClient.Api.Entitlements.EntitlementManager();
            e.EntitlementManifestFileName = "C:\\Program Files\\Intuit\\QuickBooks 2010\\Components\\PConfig\\manifest.ecml";
            
            /*
            if (e.CheckOfferingCode("068741"))
            {
                Console.WriteLine("OK");
            }

            Api.Synchronization.ISynchronizationManager ism = e.GetSynchronizationManager("103347861573284", "405697");
            object output;
            if (ism.ApplyVerbalSyncEntitlementsResponse("TOTAL_FLOATING_KEYS", "71477210", out output))
            {
                Console.WriteLine("Good - {0}", output.ToString());
            }

            Common.Data.Manifest me = new Common.Data.Manifest();
            try
            {
                Common.Data.PropertyTypeDescriptor ptd = me.GetPropertyTypeDescriptorByName("TOTAL_FLOATING_KEYS");
                Console.WriteLine(ptd.Name);
            }
            catch (Exception k)
            {

                Console.WriteLine(k.ToString());
            }
            */

            /*
            Api.Entitlements.Custom.IOfferingEntitlement1 iof = (Api.Entitlements.Custom.IOfferingEntitlement1)e.GetOfferingEntitlement("103347861573284", "405697");

            Console.WriteLine(iof.BindableResourceId);

            Common.PhoneTransaction.PhoneTransactionManager pm = new Common.PhoneTransaction.PhoneTransactionManager();
            if (pm.IsEnablementCodeValid("103347861573284", "405697", "", "493864"))
            {
                Console.WriteLine("OKOKOK");
            }
            */
            /*
            string c0 = "136471550573136405697";
            //int k0 = pm.ComputeIndex(c0);
            //Console.WriteLine("1:{0}", k0);

            System.Security.Cryptography.HashAlgorithm ha = System.Security.Cryptography.HashAlgorithm.Create("SHA1");
            System.Text.UTF8Encoding u8e = new System.Text.UTF8Encoding();
            byte[] c1 = u8e.GetBytes(c0);

            int tick = System.Environment.TickCount;

            byte[] c2 = ha.ComputeHash(c1);

            //Console.WriteLine(BitConverter.ToString(c2));
            
            for (int i = 0; i < 0x270f; i++)
            {
                c2 = ha.ComputeHash(c2);
            }

            Console.WriteLine("s: {0}", System.Environment.TickCount - tick);
            Console.WriteLine(BitConverter.ToString(c2));

            BigInteger bi = new BigInteger(c2);
            BigInteger bc = new BigInteger((Int64)0x186a0);
            BigInteger bv = bi % bc;

            Console.WriteLine("2:{0}", bv.IntValue());
            int k0 = bv.IntValue();

            //k0 = pm.ComputeIndex(c0);
            //Console.WriteLine("1:{0}", k0);
            */
            /*
            Assembly ab = Assembly.GetAssembly(typeof(BigInteger));
            Console.WriteLine(ab.FullName);
            

            try
            {
                System.Resources.ResourceManager rm = new System.Resources.ResourceManager("Intuit.Spc.Map.Features.EntitlementClient.Common.hash", ab);
                byte[] go = (byte[])(rm.GetObject("Hash Table"));

                BinaryWriter bw = new BinaryWriter(File.Open("e:\\htv10.dat", FileMode.Create));
                bw.Write(go);
                bw.Close();
                
                byte[] sum = new byte[20];

                Array.Copy(go, k0 * 20, sum, 0, 20);

                BigInteger v1 = new BigInteger(sum);
                string v2 = k0.ToString().PadLeft(5, '0');
                System.Text.UTF8Encoding v3 = new System.Text.UTF8Encoding();
                System.Security.Cryptography.HashAlgorithm v4 = System.Security.Cryptography.HashAlgorithm.Create("SHA1");
                int v5 = 0;
                bool done = false;
                m _m = new m(4);
                string cc;

                int start = Int32.Parse(Console.ReadLine());
                int end = Int32.Parse(Console.ReadLine());
                Console.WriteLine("start:{0} end{1}", start, end);

                for (int k = start; k < end; k++)
                {
                    cc = k.ToString().PadLeft(6, '0');
                    if(k % 10 == 0) Console.WriteLine(">{0}", cc);
                    //if (!_m.a_s(cc)) continue;

                    for (v5 = 0; v5 < 0x2710; v5++)
                    {
                        //if(v5 % 1000 == 0) Console.WriteLine("-{0}", v5);

                        string tt = cc + v5.ToString().PadLeft(4, '0') + v2;
                        BigInteger bt = new BigInteger(v4.ComputeHash(v3.GetBytes(tt)));

                        if (v1 == bt)
                        {
                            done = true;
                            Console.WriteLine("Done -- {0}-{1}", k, v5);
                            break;
                        }
                    }

                    if (done) break;
                }
                
            }
            catch (Exception k)
            {

                Console.WriteLine(k.ToString());
            }
            */
            
            try
            {

                Type type = typeof(Common.Security.CryptoManager);

                BindingFlags privateBindings = BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Public | BindingFlags.Static;

                MethodInfo miGetDescription = type.GetMethod("g", privateBindings);

                object retObj = miGetDescription.Invoke(null, new object[] { });

                System.Security.Cryptography.ICryptoTransform v2 = (System.Security.Cryptography.ICryptoTransform)retObj;

                FileInfo f = Common.Utilities.DirectoryUtilities.GetManifestFileLocation();

                System.IO.FileStream v1 = new System.IO.FileStream(f.FullName,
                    (System.IO.FileMode)3,
                    (System.IO.FileAccess)1,
                    (System.IO.FileShare)3
                    );

                byte[] v0 = null;

                System.Security.Cryptography.ICryptoTransform v7 = v2;

                System.Security.Cryptography.CryptoStream v3 = new System.Security.Cryptography.CryptoStream(v1, v2, (System.Security.Cryptography.CryptoStreamMode)0);

                if (v1.Length <= 0x7fffffff)
                {
                    Console.WriteLine("{0}------", v1.Length);

                    v0 = new Byte[v1.Length];

                    int len = v3.Read(v0, 0, v0.Length);

                    Console.WriteLine("size read: {0}", len);

                    string pp = System.Text.Encoding.UTF8.GetString(v0);

                    //Console.WriteLine(pp);

                    StreamWriter wp = new StreamWriter("c:\\pk.xml");
                    wp.Write(pp);
                    wp.Close();
                }

            }
            catch (Exception k)
            {

                Console.WriteLine(k.ToString());
            }
            
            Console.WriteLine("end");
            Console.Read();
        }

    }

}

