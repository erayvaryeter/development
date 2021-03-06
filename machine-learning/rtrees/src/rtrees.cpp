#include <logger/logger.h>
#include <assertion/assertion.h>
#include <file/file.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <pca/pca.h>

#include "rtrees/rtrees.h"

std::shared_ptr<base::Logger> ml::RTrees::m_logger = std::make_shared<base::Logger>();

namespace ml {

void
RTrees::AppendTrainData(std::string imageDirectory, int label, std::string imageExtension) {
	m_trainImageDirectories.emplace_back(std::move(imageDirectory));
	m_trainLabels.emplace_back(std::move(label));
	m_trainExtensions.emplace_back(std::move(imageExtension));
}

void
RTrees::AppendTestData(std::string imageDirectory, int label, std::string imageExtension) {
	m_testImageDirectories.emplace_back(std::move(imageDirectory));
	m_testLabels.emplace_back(std::move(label));
	m_testExtensions.emplace_back(std::move(imageExtension));
}

void 
RTrees::Train() {
	ASSERT(m_trainImageDirectories.size() == m_trainExtensions.size(), "Train data size is not valid", base::Logger::Severity::Error);
	ASSERT(m_trainExtensions.size() == m_trainLabels.size(), "Train data size is not valid", base::Logger::Severity::Error);
	m_preprocessor->ClearTrainImages();
	for (size_t i = 0; i < m_trainImageDirectories.size(); ++i)
		m_preprocessor->AppendTrainImagesWithLabels(m_trainImageDirectories[i], m_trainLabels[i], m_trainExtensions[i]);
	
	switch (m_method) {
	case DataExtractionMethod::PCA:
	{
		m_preprocessor->ApplyPcaTrain(m_numberOfComponents);
		break;
	}
	case DataExtractionMethod::HARRIS_CORNERS:
	{
		m_preprocessor->ApplyHarrisCornersTrain(m_numberOfComponents, m_harrisCornerParams.blockSize, m_harrisCornerParams.apertureSize, m_harrisCornerParams.k);
		break;
	}
	case DataExtractionMethod::SHI_TOMASI_CORNERS:
	{
		m_preprocessor->ApplyShiTomasiCornersTrain(m_numberOfComponents, m_shiTomasiCornerParams.maxCorners, m_shiTomasiCornerParams.qualityLevel,
			m_shiTomasiCornerParams.minDistance, m_shiTomasiCornerParams.blockSize, m_shiTomasiCornerParams.gradientSize, m_shiTomasiCornerParams.useHarris,
			m_shiTomasiCornerParams.k);
		break;
	}
	case DataExtractionMethod::SIFT:
	{
		m_preprocessor->ApplySiftTrain(m_numberOfComponents, m_siftParams.nFeatures, m_siftParams.nOctaveLayers, m_siftParams.contrastThreshold,
			m_siftParams.edgeThreshold, m_siftParams.sigma);
		break;
	}
	case DataExtractionMethod::SURF:
	{
		m_preprocessor->ApplySurfTrain(m_numberOfComponents, m_surfParams.hessianThreshold, m_surfParams.nOctaves, m_surfParams.nOctaveLayers,
			m_surfParams.extended, m_surfParams.upright);
		break;
	}
	case DataExtractionMethod::FAST:
	{
		m_preprocessor->ApplyFastTrain(m_numberOfComponents, m_fastParams.threshold, m_fastParams.nonmaxSupression);
		break;
	}
	case DataExtractionMethod::BRIEF:
	{
		m_preprocessor->ApplyBriefTrain(m_numberOfComponents, m_briefParams.bytes, m_briefParams.useOrientation);
		break;
	}
	case DataExtractionMethod::ORB:
	{
		m_preprocessor->ApplyOrbTrain(m_numberOfComponents, m_orbParams.nFeatures, m_orbParams.scaleFactor, m_orbParams.nLevels, m_orbParams.edgeThreshold, 
			m_orbParams.firstLevel, m_orbParams.WTA_K, m_orbParams.st, m_orbParams.patchSize, m_orbParams.fastThreshold);
		break;
	}
	case DataExtractionMethod::BRISK:
	{
		m_preprocessor->ApplyBriskTrain(m_numberOfComponents, m_briskParams.thresh, m_briskParams.octaves, m_briskParams.patternScale);
		break;
	}
	default: break;
	}

	auto trainData = m_preprocessor->GetTrainData();
	auto trainLabels = m_preprocessor->GetTrainLabels();
	m_rtreesPtr->train(trainData, cv::ml::ROW_SAMPLE, trainLabels);
}

void
RTrees::Test() {
	ASSERT(m_testImageDirectories.size() == m_testExtensions.size(), "Test data size is not valid", base::Logger::Severity::Error);
	ASSERT(m_testExtensions.size() == m_testLabels.size(), "Test data size is not valid", base::Logger::Severity::Error);
	m_preprocessor->ClearTestImages();
	for (size_t i = 0; i < m_testImageDirectories.size(); ++i)
		m_preprocessor->AppendTestImagesWithLabels(m_testImageDirectories[i], m_testLabels[i], m_testExtensions[i]);
	
	switch (m_method) {
	case DataExtractionMethod::PCA:
	{
		m_preprocessor->ApplyPcaTest(m_numberOfComponents);
		break;
	}
	case DataExtractionMethod::HARRIS_CORNERS:
	{
		m_preprocessor->ApplyHarrisCornersTest(m_numberOfComponents, m_harrisCornerParams.blockSize, m_harrisCornerParams.apertureSize, m_harrisCornerParams.k);
		break;
	}
	case DataExtractionMethod::SHI_TOMASI_CORNERS:
	{
		m_preprocessor->ApplyShiTomasiCornersTest(m_numberOfComponents, m_shiTomasiCornerParams.maxCorners, m_shiTomasiCornerParams.qualityLevel, 
			m_shiTomasiCornerParams.minDistance, m_shiTomasiCornerParams.blockSize, m_shiTomasiCornerParams.gradientSize, m_shiTomasiCornerParams.useHarris,
			m_shiTomasiCornerParams.k);
		break;
	}
	case DataExtractionMethod::SIFT:
	{
		m_preprocessor->ApplySiftTest(m_numberOfComponents, m_siftParams.nFeatures, m_siftParams.nOctaveLayers, m_siftParams.contrastThreshold,
			m_siftParams.edgeThreshold, m_siftParams.sigma);
		break;
	}
	case DataExtractionMethod::SURF:
	{
		m_preprocessor->ApplySurfTest(m_numberOfComponents, m_surfParams.hessianThreshold, m_surfParams.nOctaves, m_surfParams.nOctaveLayers, 
			m_surfParams.extended, m_surfParams.upright);
		break;
	}
	case DataExtractionMethod::FAST:
	{
		m_preprocessor->ApplyFastTest(m_numberOfComponents, m_fastParams.threshold, m_fastParams.nonmaxSupression);
		break;
	}
	case DataExtractionMethod::BRIEF:
	{
		m_preprocessor->ApplyBriefTest(m_numberOfComponents, m_briefParams.bytes, m_briefParams.useOrientation);
		break;
	}
	case DataExtractionMethod::ORB:
	{
		m_preprocessor->ApplyOrbTest(m_numberOfComponents, m_orbParams.nFeatures, m_orbParams.scaleFactor, m_orbParams.nLevels, m_orbParams.edgeThreshold,
			m_orbParams.firstLevel, m_orbParams.WTA_K, m_orbParams.st, m_orbParams.patchSize, m_orbParams.fastThreshold);
		break;
	}
	case DataExtractionMethod::BRISK:
	{
		m_preprocessor->ApplyBriskTest(m_numberOfComponents, m_briskParams.thresh, m_briskParams.octaves, m_briskParams.patternScale);
		break;
	}
	default: break;
	}

	auto testData = m_preprocessor->GetTestData();
	auto testLabels = m_preprocessor->GetTestLabels();

	int correctClassify = 0;
	int wrongClassify = 0;

	for (int i = 0; i < testData.rows; i++) {
		cv::Mat neighbors;
		float response = m_rtreesPtr->predict(testData.row(i));
		int expected = testLabels.at<int>(i);
		if (expected == response)
			correctClassify++;
		else
			wrongClassify++;
		std::cout << "Expected: " << expected << " - Predicted: " << response << " Neighbors: " << neighbors << std::endl;
	}

	std::cout << "-------------------------------------------------------------------------------" << std::endl;

	std::cout << "Correctly classified % " << static_cast<float>(correctClassify) / static_cast<float>(correctClassify + wrongClassify)
		* 100 << std::endl;

	std::cout << "Wrongly classified % " << static_cast<float>(wrongClassify) / static_cast<float>(correctClassify + wrongClassify)
		* 100 << std::endl;
}

}