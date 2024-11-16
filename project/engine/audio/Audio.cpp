#include "Audio.h"

#include "assert.h"

Audio* Audio::instance = nullptr;

Audio* Audio::GetInstance()
{
	if (instance == nullptr) {
		instance = new Audio;
	}

	return instance;
}

void Audio::Initialize()
{
	HRESULT result;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);

}
void Audio::Finalize()
{

	SoundUnload();

	delete instance;
	instance = nullptr;

}


void Audio::SoundLoadWave(const char* filename)
{

	if (soundDatas.find(filename) != soundDatas.end())
	{
		return;
	}

	HRESULT result;

	/*-----------------------------
	* ファイルをオープン
	-----------------------------*/

	// ファイル入力ストリームのインスタンス
	std::ifstream file;

	// .wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);

	// ファイルオープン失敗を検出する
	assert(file.is_open());

	/*-----------------------------
	* .wavデータの読み込み
	-----------------------------*/

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	// Formatチャンクの読み込み
	FormatChunk format = {};

	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;

	//// JUNKチャンクを検出した場合
	while (true) {
		// チャンクヘッダーを読み込む
		file.read((char*)&data, sizeof(data));

		// 読み込みエラーが発生した場合、ファイルを閉じて終了
		if (file.eof() || !file) {
			assert(0);  // データチャンクが見つからなかった
			break;
		}

		// Dataチャンクを検出した場合
		if (strncmp(data.id, "data", 4) == 0) {
			break;  // Dataチャンクが見つかったのでループを抜ける
		}

		// JUNKチャンクやその他のチャンクの場合、読み飛ばす
		file.seekg(data.size, std::ios_base::cur);
	}

	// Dataチャンクのデータ部(波系データ)の読み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// waveファイルを閉じる
	file.close();

	/*-----------------------------
	* 読み込んだ音声データを返す
	-----------------------------*/

	// returnする為の音声データ
	
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	soundDatas[filename] = soundData;
}

void Audio::SoundUnload()
{
	// バッファのメモリを解放
	/*if (soundData.pBuffer)
	{
		delete[] soundData.pBuffer;
		soundData.pBuffer = nullptr;
		soundData.bufferSize = 0;
		soundData.wfex = {};
	}*/

	for (auto& [key, soundData] : soundDatas)
	{
		if (soundData.pBuffer)
		{
			delete[] soundData.pBuffer;
			soundData.pBuffer = nullptr;
			soundData.bufferSize = 0;
			soundData.wfex = {};
		}
	}
	// 必要であればマップをクリア
	soundDatas.clear();
}

void Audio::SoundPlayWave(const char* filename)
{
	HRESULT result;

	// 波系フォーマットを元にSourceVoiceの生成
	
	if (soundDatas.find(filename) != soundDatas.end())
	{

		result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundDatas.find(filename)->second.wfex);
		assert(SUCCEEDED(result));

		// 再生する波形データの設定
		XAUDIO2_BUFFER buf{};
		buf.pAudioData = soundDatas.find(filename)->second.pBuffer;
		buf.AudioBytes = soundDatas.find(filename)->second.bufferSize;
		buf.Flags = XAUDIO2_END_OF_STREAM;

		// 波形データの再生
		result = pSourceVoice->SubmitSourceBuffer(&buf);
		result = pSourceVoice->Start();

		return;
	}


	assert(0);
}