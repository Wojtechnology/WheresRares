using UnityEngine;
using System.Runtime.InteropServices;

public static class CVPlugin {

	[DllImport ("WheresRares")]
	public extern static int AddTwoIntegers (int a, int b);

	[DllImport ("WheresRares")]
	public extern static double LogBase10 (double a);

	[DllImport ("WheresRares")]
	public extern static int GetArray (int[] a);

	[DllImport ("WheresRares")]
	public extern static System.IntPtr ArrayTest (int[] a);

}
