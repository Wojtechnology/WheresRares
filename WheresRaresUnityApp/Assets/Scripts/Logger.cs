using System;
using System.Runtime.InteropServices;
using UnityEngine;

public static class Logger {

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	public delegate void MyDelegate(string str);

	public static void CallBackFunction(string str)
	{
		Debug.Log("::CallBaaaaaaack : " + str);
	}


}
	
