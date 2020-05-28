/*
Input MediaBuffer[256]:
 ____________________________
|0|1|2|    ....          255|
|____________________________


Given a MediaBuffer, the decoded frame can be constructed independently
(That is, without having to refer to previous or next frame)

Output DecodedFrames[256]:
 ____________________________________
|0 |1 |2 |      ....             255|
|____________________________________
*/

static size_t kMediaBufferSize = 256;
static size_t gMediaBufferCount = 0;
MediaBuffer   gMediaBuffers[kMediaBufferSize] = {};
DecodedFrame  gDecodedFrames[kMediaBufferSize] = {};

/*
 * Reads the MediaBuffers (gMediaBuffers) from the containers and places
 * 256 buffers into MediaBuffer (gDecodedFrames).
 */
bool RetrieveMediaBuffers() {
}

/*
 * Decodes the MediaBuffer at the index bufferCount
 * and saves the decoded frame into Decoded Frames
 */
void DecodeFrame(size_t bufferCount) {
}

void* WorkerThread(void* arg) { 
    size_t bufferCount = 0;
    while (bufferCount < kMediaBufferSize - 1) {
        bufferCount = gMediaBufferCount++;
        DecodeFrame(bufferCount);
    }
} 

void ProcessMediaBuffers() {
    pthread_t thread_id; 
    pthread_create(&thread_id, NULL, WorkerThread, NULL); 
    size_t bufferCount = 0;
    while (bufferCount < kMediaBufferSize - 1) {
        bufferCount = gMediaBufferCount++;
        DecodeFrame(bufferCount);
    }
    pthread_join(thread_id, NULL); 
}

int main() {
    bool eos = false;
    do {
        eos = RetrieveMediaBuffers();
        ProcessMediaBuffers();
        RenderFrames();
    } while (!eos);

    return 0;
}
