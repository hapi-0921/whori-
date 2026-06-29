#include"Camera.h"

//指定方向を向く
void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus,
	const DirectX::XMFLOAT3& up)
{
	//視点、注視点、上方向から
	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye,Focus,Up);
	DirectX::XMStoreFloat4x4(&view, View);


	//====================カメラの方向を取り出す====================
	
	//thisポインタ＝この関数を呼び出した“オブジェクト（インスタンス）”がthis入る
	//でも型は Camera*　=>　Cameraに定義されてないメンバにはアクセスできない

	//DirectX::XMFLOAT4X4 view;（ヘッダーより）
	//XMFLOAT4X4構造体の中身
	//float _11,_12,_13,_14;	X軸ベクトル				
	//float _21,_22,_23,_24;	Y軸ベクトル
	//float _31,_32,_33,_34;	Z軸ベクトル
	//float _41,_42,_43,_44;	位置

	//例）charactorのtransforn
	//DirectX::XMFLOAT4X4 transform = {
	//  1, 0, 0, 0,	  X軸ベクトル		
	//  0, 1, 0, 0,	  Y軸ベクトル
	//  0, 0, 1, 0,	  Z軸ベクトル
	//  0, 0, 0, 1	  位置
	// };


	this->right.x = view._11;
	this->right.y = view._21;
	this->right.z = view._31;

	this->up.x = view._12;
	this->up.y = view._22;
	this->up.z = view._32;
	
	this->front.x = view._13;
	this->front.y = view._23;
	this->front.z = view._33;
	//=============================================================
	
	//==============視点、注視点を保存=============
	this->eye = eye;
	this->focus = focus;
	//=============================================
}

//パースペクティブ設定
void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
	//画角(fov)、画面比率(aspect)、クリップ距離(near,far)からプロジェクション行列を作成
	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&projection, Projection);
}