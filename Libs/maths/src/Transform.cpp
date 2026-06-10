#include "../includes/Math.h"

Transform::Transform(Vector3D _pos, Vector3D _eulerRot, Vector3D _scale)
{
    pos = _pos;
    eulerRot = _eulerRot;
    scale = _scale;
}
