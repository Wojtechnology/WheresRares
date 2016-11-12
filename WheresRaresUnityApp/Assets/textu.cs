using UnityEngine;
using System.Collections;

public class textu : MonoBehaviour {
	WebCamTexture webcamTexture;

	// Use this for initialization
	IEnumerator Start () {
		yield return Application.RequestUserAuthorization (UserAuthorization.WebCam | UserAuthorization.Microphone);
		if (Application.HasUserAuthorization (UserAuthorization.WebCam | UserAuthorization.Microphone)) {
			string camera_name = WebCamTexture.devices [0].name;
			webcamTexture = new WebCamTexture ();
			webcamTexture.deviceName = camera_name;
			Renderer renderer = this.GetComponentInChildren<Renderer> ();
			renderer.material.mainTexture = webcamTexture;
			webcamTexture.Play ();
		} else {
		}
	}

	// Update is called once per frame
	void Update () {
	}
}
