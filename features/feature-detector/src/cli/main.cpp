#include "HarrisCornerDetector.h"
#include "ShiTomasiCornerDetector.h"
#include "SIFT.h"
#include "SURF.h"
#include "FAST.h"
#include "BRIEF.h"
#include "ORB.h"
#include "BRISK.h"

enum class DetectorType {
	HARRIS_CORNER_DETECTOR = 0,
	SHI_TOMASI_CORNER_DETECTION = 1,
	SIFT = 2,
	SURF = 3,
	FAST = 4,
	BRIEF = 5,
	ORB = 6,
	BRISK = 7
};

DetectorType
ConvertDetectorType(std::string detectorType) {
	if (detectorType == "HarrisCorner") {
		return DetectorType::HARRIS_CORNER_DETECTOR;
	}
	else if (detectorType == "ShiTomasi") {
		return DetectorType::SHI_TOMASI_CORNER_DETECTION;
	}
	else if (detectorType == "SIFT") {
		return DetectorType::SIFT;
	}
	else if (detectorType == "SURF") {
		return DetectorType::SURF;
	}
	else if (detectorType == "FAST") {
		return DetectorType::FAST;
	}
	else if (detectorType == "BRIEF") {
		return DetectorType::BRIEF;
	}
	else if (detectorType == "ORB") {
		return DetectorType::ORB;
	}
	else if (detectorType == "BRISK") {
		return DetectorType::BRISK;
	}
	else {
		std::cout << "Given detector type string is invalid, exitting" << std::endl;
		exit(0);
	}
}

int main(int argc, char** argv) {
	cxxopts::Options options("Feature Detector");
	options.add_options()
		("i,image", "Input image path", cxxopts::value<std::string>()->default_value("../../../../features/feature-detector/resource/Chessboard.jpg"))
		("d,detector-type", "Types: HarrisCorner - ShiTomasi - SIFT - SURF - FAST - BRIEF - ORB", cxxopts::value<std::string>()->default_value("HarrisCorner"))
		("h,help", "Print usage");

	auto result = options.parse(argc, argv);
	if (result.count("help")) {
		std::cout << options.help() << std::endl;
		exit(0);
	}

	std::string imagePath;
	if (result.count("image")) {
		imagePath = result["image"].as<std::string>();
		if (!base::File::FileExists(imagePath)) {
			std::cout << "Image with given path does not exist" << std::endl;
			exit(0);
		}
		auto ext = base::File::GetFileExtension(imagePath);
		if (ext != ".jpg" && ext != ".jpeg" && ext != ".png" && ext != ".bmp") {
			std::cout << "Given image path extension is not valid" << std::endl;
			exit(0);
		}
	}
	else {
		imagePath = result["image"].as<std::string>();
	}

	DetectorType dt;
	dt = ConvertDetectorType(result["detector-type"].as<std::string>());

	switch (dt) {
	case DetectorType::HARRIS_CORNER_DETECTOR: 
	{
		auto detector = std::make_shared<HarrisCornerDetector>(imagePath);
		detector->Run();
		break;
	}
	case DetectorType::SHI_TOMASI_CORNER_DETECTION:
	{
		auto detector = std::make_shared<ShiTomasiCornerDetector>(imagePath);
		detector->Run();
		break;
	}
	case DetectorType::SIFT:
	{
		auto detector = std::make_shared<SIFTDetector>(imagePath);
		detector->Run();
		break;
	}
	case DetectorType::SURF:
	{
		auto detector = std::make_shared<SURFDetector>(imagePath);
		detector->Run();
		break;
	}
	case DetectorType::FAST:
	{
		auto detector = std::make_shared<FASTDetector>(imagePath);
		detector->Run();
		break;
	}
	case DetectorType::BRIEF:
	{
		auto detector = std::make_shared<BRIEFDetector>(imagePath);
		detector->Run();
		break;
	}
	case DetectorType::ORB:
	{
		auto detector = std::make_shared<ORBDetector>(imagePath);
		detector->Run();
		break;
	}
	case DetectorType::BRISK:
	{
		auto detector = std::make_shared<BRISKDetector>(imagePath);
		detector->Run();
		break;
	}
	default: break;
	}

	return 0;
}