using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class TurretController : MonoBehaviour
{

    public Transform turretTransform;
    private Vector3 _direction;

    // Start is called before the first frame update
    void Start()
    {
        _direction = turretTransform.forward;
    }

    // Update is called once per frame
    void Update()
    {
        _direction += Vector3.up*Input.GetAxis("Mouse X");
        turretTransform.rotation = Quaternion.Euler(_direction);
    }
}
