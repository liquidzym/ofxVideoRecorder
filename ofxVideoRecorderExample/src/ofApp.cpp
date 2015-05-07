#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	sampleRate = 44100;
	channels = 1;

	ofSetFrameRate(60);
	ofSetLogLevel(OF_LOG_VERBOSE);
	vidGrabber.setDesiredFrameRate(30);
	vidGrabber.initGrabber(1920, 1080);
	//#ifdef TARGET_WIN32
		//vidRecorder.setFfmpegLocation("D:\\ffmpeg\\bin\\ffmpeg"); // use this is you have ffmpeg installed in your data folder
	//#endif
	fileName = "testMovie";
	fileExt = ".mp4"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats

	// override the default codecs if you like
	// run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
	vidRecorder.setVideoCodec("mpeg4");
	vidRecorder.setVideoBitrate("800k");
	// vidRecorder.setAudioCodec("alac");
	vidRecorder.setAudioBitrate("192k");

    soundStream.listDevices();

	soundStream.setDeviceID(2);
	soundStream.setup(this, 0, channels, sampleRate, 256, 4);

	ofSetWindowShape(vidGrabber.getWidth(), vidGrabber.getHeight());
	bRecording = false;
	ofEnableAlphaBlending();

}

void ofApp::exit(){
	vidRecorder.close();
}

//--------------------------------------------------------------
void ofApp::update(){
	vidGrabber.update();
	if (vidGrabber.isFrameNew() && bRecording){
		vidRecorder.addFrame(vidGrabber.getPixelsRef());
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255, 255, 255);
	vidGrabber.draw(0, 0);

	stringstream ss;
	ss << "video queue size: " << vidRecorder.getVideoQueueSize() << endl
		<< "audio queue size: " << vidRecorder.getAudioQueueSize() << endl
		<< "FPS: " << ofGetFrameRate() << endl
		<< (bRecording ? "pause" : "start") << " recording: r" << endl
		<< (bRecording ? "close current video file: c" : "") << endl;

	ofSetColor(0, 0, 0, 100);
	ofRect(0, 0, 260, 75);
	ofSetColor(255, 255, 255);
	ofDrawBitmapString(ss.str(), 15, 15);

	if (bRecording){
		ofSetColor(255, 0, 0);
		ofCircle(ofGetWidth() - 20, 20, 5);
	}
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{
	if (bRecording){
		vidRecorder.addAudioSamples(input, bufferSize, nChannels);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'r'){
		bRecording = !bRecording;
		if (bRecording && !vidRecorder.isInitialized()) {
			vidRecorder.setup(fileName + ofGetTimestampString()/* + fileExt*/, vidGrabber.getWidth(), vidGrabber.getHeight(), 30, sampleRate, channels);
			//          vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, vidGrabber.getWidth(), vidGrabber.getHeight(), 30); // no audio
			//            vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, 0,0,0, sampleRate, channels); // no video
			//          vidRecorder.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, sampleRate, channels, "-vcodec mpeg4 -b 1600k -acodec mp2 -ab 128k -f mpegts udp://localhost:1234"); // for custom ffmpeg output string (streaming, etc)
		}
	}
	if (key == 'c'){
		bRecording = false;
		vidRecorder.close();
	}

	if (key == 'b')
	{
		// Ñ¹ËõÒ»¸ö°æ±¾
		//ffmpegThread.waitForThread();
		stringstream finalCmd;

		finalCmd << " -i " << vidRecorder.getMoviePath() + fileExt << " -vf scale=160:120 " << vidRecorder.getMoviePath() << "_small" << fileExt;
		ofLogNotice("FFMpeg Resize") << " -------------------------- ";
		ofLogNotice("FFMpeg Resize") << finalCmd.str();

	
		vidRecorder.runCustomScript(finalCmd.str());
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
