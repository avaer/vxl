#include <emscripten.h>
#include "util.h"
#include "compose.h"
#include "light.h"
#include "heightfield.h"
#include "cull.h"
#include "noiser.h"
// #include <iostream>

extern "C" {

int main() {
  // std::cout << "main" << "\n";
  initUtil();
}

EMSCRIPTEN_KEEPALIVE Noiser *make_noiser(int seed) {
  return new Noiser(seed);
}

EMSCRIPTEN_KEEPALIVE void destroy_noiser(Noiser *noiser) {
  delete noiser;
}

EMSCRIPTEN_KEEPALIVE void noiser_apply(Noiser *noiser, int ox, int oz, unsigned char *biomes, unsigned char *temperature, unsigned char *humidity, bool fillBiomes, float *elevations, bool fillElevations, float *ethers, bool fillEther, float *water, float *lava, bool fillLiquid, float *newEther, unsigned int numNewEthers) {
  noiser->apply(ox, oz, biomes, temperature, humidity, fillBiomes, elevations, fillElevations, ethers, fillEther, water, lava, fillLiquid, newEther, numNewEthers);
}

EMSCRIPTEN_KEEPALIVE void noiser_fill(Noiser *noiser, int ox, int oz, unsigned char *biomes, float *elevations, float *ethers, float *water, float *lava, float *positions, unsigned int *indices, unsigned int *attributeRanges, unsigned int *indexRanges, float *staticHeightfield, float *colors, unsigned char *peeks) {
  noiser->fill(ox, oz, biomes, elevations, ethers, water, lava, positions, indices, attributeRanges, indexRanges, staticHeightfield, colors, peeks);
}

EMSCRIPTEN_KEEPALIVE void objectize(
  void *objectsSrc, void *vegetationsSrc, void *geometries, unsigned int *geometryIndex,
  unsigned int *blocks, unsigned int *blockTypes, int *dims, unsigned char *transparentVoxels, unsigned char *translucentVoxels, float *faceUvs, float *shift,
  float *positions, float *uvs, unsigned char *ssaos, float *frames, float *objectIndices, unsigned int *indices, unsigned int *objects,
  unsigned int *result
) {
  unsigned int positionIndex[NUM_CHUNKS_HEIGHT];
  unsigned int uvIndex[NUM_CHUNKS_HEIGHT];
  unsigned int ssaoIndex[NUM_CHUNKS_HEIGHT];
  unsigned int frameIndex[NUM_CHUNKS_HEIGHT];
  unsigned int objectIndexIndex[NUM_CHUNKS_HEIGHT];
  unsigned int indexIndex[NUM_CHUNKS_HEIGHT];
  unsigned int objectIndex[NUM_CHUNKS_HEIGHT];

  compose(
    objectsSrc, vegetationsSrc, geometries, geometryIndex,
    blocks, blockTypes, dims, transparentVoxels, translucentVoxels, faceUvs, shift,
    positions, uvs, ssaos, frames, objectIndices, indices, objects,
    positionIndex, uvIndex, ssaoIndex, frameIndex, objectIndexIndex, indexIndex, objectIndex
  );

  for (unsigned int i = 0; i < NUM_CHUNKS_HEIGHT; i++) {
    result[NUM_CHUNKS_HEIGHT * 0 + i] = positionIndex[i];
    result[NUM_CHUNKS_HEIGHT * 1 + i] = uvIndex[i];
    result[NUM_CHUNKS_HEIGHT * 2 + i] = ssaoIndex[i];
    result[NUM_CHUNKS_HEIGHT * 3 + i] = frameIndex[i];
    result[NUM_CHUNKS_HEIGHT * 4 + i] = objectIndexIndex[i];
    result[NUM_CHUNKS_HEIGHT * 5 + i] = indexIndex[i];
    result[NUM_CHUNKS_HEIGHT * 6 + i] = objectIndex[i];
  }
}

EMSCRIPTEN_KEEPALIVE bool lght(
  int ox, int oz, int minX, int maxX, int minY, int maxY, int minZ, int maxZ, unsigned int relight,
  float **lavaArray, float **objectLightsArray, float **etherArray, unsigned int **blocksArray, unsigned char **lightsArray
) {
  return light(ox, oz, minX, maxX, minY, maxY, minZ, maxZ, (bool)relight, lavaArray, objectLightsArray, etherArray, blocksArray, lightsArray);
}

EMSCRIPTEN_KEEPALIVE void lghtmap(int ox, int oz, float *positions, unsigned int numPositions, float *staticHeightfield, unsigned char *lights, unsigned char *skyLightmaps, unsigned char *torchLightmaps) {
  lightmap(ox, oz, positions, numPositions, staticHeightfield, lights, skyLightmaps, torchLightmaps);
}

EMSCRIPTEN_KEEPALIVE void blockfield(unsigned int *blocks, unsigned char *blockfield) {
  genBlockfield(blocks, blockfield);
}

EMSCRIPTEN_KEEPALIVE void cllTerrain(float *hmdPosition, float *projectionMatrix, float *matrixWorldInverse, int frustumCulled, int *mapChunkMeshes, unsigned int numMapChunkMeshes, int *groups, int *groups2, unsigned int *groupIndices) {
  cullTerrain(hmdPosition, projectionMatrix, matrixWorldInverse, frustumCulled != 0, mapChunkMeshes, numMapChunkMeshes, groups, groups2, groupIndices[0], groupIndices[1]);
}

EMSCRIPTEN_KEEPALIVE unsigned int cllObjects(float *hmdPosition, float *projectionMatrix, float *matrixWorldInverse, int frustumCulled, int *mapChunkMeshes, unsigned int numMapChunkMeshes, int *groups) {
  return cullObjects(hmdPosition, projectionMatrix, matrixWorldInverse, frustumCulled != 0, mapChunkMeshes, numMapChunkMeshes, groups);
}

EMSCRIPTEN_KEEPALIVE unsigned int collideBoxEther(int dims[3], float *potential, float *boxSpec) {
  return (unsigned int)collideBoxEther(dims, potential, boxSpec);
}

}
