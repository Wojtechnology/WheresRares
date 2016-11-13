using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;

public class CameraLoader : MonoBehaviour {

	// Use this for initialization
	WebCamTexture camera;
	Color32[] map;

	bool isTracking;
	void Start () {
		//var deviceName = WebCamTexture.devices[0].name;
		//camera = new WebCamTexture(deviceName, 1280, 720, 30);
		camera = new WebCamTexture();
		//Renderer renderer = GetComponent<Renderer>();

		// set the size of the plane
		//var height = Camera.main.orthographicSize * 2;
		//var width = height * (Screen.width / Screen.height);
		//transform.localScale = new Vector3(width, height, 1);

		//renderer.material.mainTexture = camera;
		camera.Play();

		isTracking = false;

		Logger.MyDelegate callback_delegate = new Logger.MyDelegate( Logger.CallBackFunction );
		// Convert callback_delegate into a function pointer that can be
		// used in unmanaged code.
		IntPtr intptr_delegate =
			Marshal.GetFunctionPointerForDelegate(callback_delegate);
		// Call the API passing along the function pointer.
		CVPlugin.SetDebugFunction( intptr_delegate );
	}

	// Update is called once per frame
	bool first = true;
	void Update () {
		// TODO: Move this out
		if (camera.width <= 16) {
			Debug.Log ("Still waiting another frame for correct info...");
			return;
		} else if (first) {
			map = new Color32[camera.width * camera.height];
			CVPlugin.InitCameraPlugin (camera.height, camera.width);
			first = false;
		}
		camera.GetPixels32 (map);

		if (isTracking) {
			IntPtr rect = CVPlugin.Track (CameraUtils.Color32ArrayToByteArray (map), camera.height, camera.width);
			int[] a = new int[4];
			Marshal.Copy (rect, a, 0, 4);
			Debug.Log ("woot " + a[0] + " " + a[1] + " " + a[2] + " " + a[3]);
			return;
		}
//		int[] test = { 1, 2, 3, 4 };
//
//		int[] a = new int[3];
//		IntPtr b = CVPlugin.GetBoundingBox (test);
//		System.Runtime.InteropServices.Marshal.Copy (b,a,0,3);
//		Debug.Log ("woot" + a[0]);

//		int[] a = new int[4];
//		IntPtr b = CVPlugin.GetQuickBoundingBox (map[120]);
//		System.Runtime.InteropServices.Marshal.Copy (b,a,0,3);
//		Debug.Log ("woot" + a[0]);

//		byte[] test = CameraUtils.Color32ArrayToByteArray (map);
//		for (int i = 0; i < 16; i++) {
//			Debug.Log(test[i]);
//		}

//		byte[] byte_map = CameraUtils.Color32ArrayToByteArray (map);
//		int[] a = new int[4];
//		IntPtr b = CVPlugin.GetQuickerBoundingBox (byte_map, camera.height, camera.width);
//		Marshal.Copy (b,a,0,3);
//		Debug.Log ("woot " + a[0] + " " + a[1] + " " + a[2] + " " + a[3]);

		if (CVPlugin.DetectPerson (CameraUtils.Color32ArrayToByteArray (map), camera.height, camera.width)) {
			Debug.Log ("There is a person!");
			isTracking = true;
		}
	}
}