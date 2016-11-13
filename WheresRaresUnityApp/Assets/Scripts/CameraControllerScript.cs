using UnityEngine;
using System.Collections;

public class CameraControllerScript : MonoBehaviour {

	private WebCamTexture camera;
	void Start () {
		WebCamTexture camera = new WebCamTexture();
		camera.Play();
	}

	void Update () {
		int[] a = {1,2,3};
		int[] a2 = new int[3];
		System.IntPtr b = CVPlugin.ArrayTest (a);
		System.Runtime.InteropServices.Marshal.Copy (b,a2,0,3);
		foreach (int i in a2) {
			Debug.Log (i);
		}
			
		// for obj in objects
		// move or delete

		// check for new objects
		// if more than n objects present
		// create new object at pos x y z
	}
}
