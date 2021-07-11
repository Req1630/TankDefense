#include "Shader.h"

#include <mutex>

namespace
{
	std::mutex	g_D3DX11CompMutex;
	std::mutex	g_D3DX11VSCreateMutex;
	std::mutex	g_D3DX11PSCreateMutex;
};

namespace shader
{

//----------------------------------.
// �V�F�[�_�[�Ǎ��֐�.
//----------------------------------.
HRESULT InitShader(
	const char* filePath,
	const char* entryPoint,
	const char* shaderModel,
	UINT compileFlag,
	ID3DBlob** ppCompiledShader,
	ID3DBlob** ppErrors )
{
	std::unique_lock<std::mutex> lock(g_D3DX11CompMutex);

	return D3DX11CompileFromFile(
		filePath,			// �V�F�[�_�[�t�@�C���p�X.
		nullptr,			// �}�N����`�̃|�C���^�[ : �I�v�V����.
		nullptr,			// �C���N���[�h�t�@�C�����������邽�߂̃|�C���^ : �I�v�V����.
		entryPoint,			// �V�F�[�_�[�̃G���g���[�|�C���g�֐���.
		shaderModel,		// �V�F�[�_�[���f���̖��O.
		compileFlag,		// �V�F�[�_�[�R���p�C���t���O.
		0,					// �G�t�F�N�g�R���p�C���t���O (0����).
		nullptr,			// �X���b�h�|���v�C���^�[�t�F�C�X�ւ̃|�C���^�[.
		ppCompiledShader,	// �R���p�C�����ꂽ�V�F�[�_�[�̃f�[�^ (out).
		ppErrors,			// �R���p�C�����̃G���[�o�� (out).
		nullptr );			// �߂�l�̃|�C���^.
}

//----------------------------------.
// �o�[�e�b�N�X�V�F�[�_�[�쐬�֐�.
//----------------------------------.
HRESULT CreateVertexShader(
	ID3D11Device*			pDevice11,
	ID3DBlob*				pCompiledShader,
	ID3D11VertexShader**	pVertexShader )
{
	std::unique_lock<std::mutex> lock(g_D3DX11VSCreateMutex);
	
	return pDevice11->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		nullptr,
		pVertexShader );
}

//----------------------------------.
// �s�N�Z���X�V�F�[�_�[�쐬�֐�.
//----------------------------------.
HRESULT CreatePixelShader(
	ID3D11Device*		pDevice11,
	ID3DBlob*			pCompiledShader,
	ID3D11PixelShader**	pPixelShader )
{
	std::unique_lock<std::mutex> lock(g_D3DX11PSCreateMutex);

	return pDevice11->CreatePixelShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		nullptr,
		pPixelShader );
}

//----------------------------------.
// �C���v�b�g���C�A�E�g�쐬�֐�.
//----------------------------------.
HRESULT CreateInputLayout(
	ID3D11Device*				pDevice11,
	D3D11_INPUT_ELEMENT_DESC*	pLayout,
	UINT						numElements,
	ID3DBlob*					pCompiledShader, 
	ID3D11InputLayout**			m_pVertexLayout )
{
	return pDevice11->CreateInputLayout(
		pLayout,
		numElements,
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		m_pVertexLayout );
}

//----------------------------------.
// �R���X�^���g�o�b�t�@�쐬�֐�.
//----------------------------------.
HRESULT CreateConstantBuffer( 
	ID3D11Device*		pDevice11,
	ID3D11Buffer**		pConstantBuffer,
	UINT				size )
{
	D3D11_BUFFER_DESC cb;

	cb.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth			= size;
	cb.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags			= 0;
	cb.StructureByteStride	= 0;
	cb.Usage				= D3D11_USAGE_DYNAMIC;

	return pDevice11->CreateBuffer( &cb, nullptr, pConstantBuffer );
}

//----------------------------------.
// POSITION�̎擾.
//----------------------------------.
D3D11_INPUT_ELEMENT_DESC GetPositionInputElement()
{
	return
	{
		"POSITION",						// �ʒu.
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,	// DXGI�̃t�H�[�}�b�g(32bit float�^*3).
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,	// �f�[�^�̊J�n�ʒu.
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	};
}

//----------------------------------.
// TEXCOORD�̎擾.
//----------------------------------.
D3D11_INPUT_ELEMENT_DESC GetTexcoordInputElement()
{
	return
	{
		"TEXCOORD",						// �e�N�X�`���ʒu.
		0,
		DXGI_FORMAT_R32G32_FLOAT,		// DXGI�̃t�H�[�}�b�g(32bit float�^*3).
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,	// �f�[�^�̊J�n�ʒu.
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	};
}

};