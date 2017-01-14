using UnityEngine;
using System.Runtime.InteropServices;
using System;

public static class CVPlugin {

	[DllImport ("CVCode")]
	public static extern void SetDebugFunction( IntPtr fp );

	[DllImport ("CVCode")]
	public extern static void InitCameraPlugin (int height, int width);

	[DllImport ("CVCode")]
	public extern static bool DetectPerson (byte[] map, int height, int width);

	[DllImport ("CVCode")]
	public extern static System.IntPtr Track (byte[] map, int height, int width);
}
