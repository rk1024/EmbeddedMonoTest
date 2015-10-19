using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbeddedApp {
  public struct MyStruct {
    public int x;
  }

  public class EmbeddedApp {
    public static void Main(string[] args) {
      Console.WriteLine("It lives!  Hello from C#!");

      dynamic x = 5;

      Console.WriteLine("Testing 'dynamic': x = " + x);

      x = $"Hello from {5}!";

      Console.WriteLine("Testing 'dynamic' and $\"...\": x = " + x);

      Console.WriteLine("Testing internal calls: Test() = " + Test());

      MyStruct s;

      Console.WriteLine("Testing internal calls with struct marshalling: GetMyStruct().x = " + (s = GetMyStruct()).x);

      Console.WriteLine("Testing internal calls with two-way struct marshalling: IncrementMyStruct(s).x = " + IncrementMyStruct(s).x);
    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    extern static string Test();

    [MethodImpl(MethodImplOptions.InternalCall)]
    extern static MyStruct GetMyStruct();

    [MethodImpl(MethodImplOptions.InternalCall)]
    extern static MyStruct IncrementMyStruct(MyStruct myStruct);
  }
}
