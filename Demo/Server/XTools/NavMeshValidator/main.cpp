#include "ValidatorHeader.h"

namespace {
    struct CommandLineOptions {
        std::string _NavMeshFilePath;
        std::string _CasesFilePath;
        std::string _ReportFilePath;
    };

    static bool LoadJsonFile(const std::string& filePath, Json::Value& root, std::string& error) {
        std::ifstream input(filePath, std::ios::in | std::ios::binary);
        if (!input.is_open()) {
            error = "open file failed: " + filePath;
            return false;
        }

        Json::CharReaderBuilder builder;
        builder["collectComments"] = false;
        if (!Json::parseFromStream(builder, input, &root, &error)) {
            error = "parse json failed: " + error;
            return false;
        }

        return true;
    }

    static bool SaveJsonFile(const std::string& filePath, const Json::Value& root, std::string& error) {
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "  ";

        std::ofstream output(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!output.is_open()) {
            error = "open output file failed: " + filePath;
            return false;
        }

        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(root, &output);
        return true;
    }

    static bool ParseCommandLineOptions(const int argc, const char** argv, CommandLineOptions& options) {
        for (int index = 1; index < argc; index++) {
            const char* currentArgument = argv[index];
            if (strncmp(currentArgument, "--navmesh=", 10) == 0) {
                options._NavMeshFilePath = currentArgument + 10;
            }
            else if (strncmp(currentArgument, "--cases=", 8) == 0) {
                options._CasesFilePath = currentArgument + 8;
            }
            else if (strncmp(currentArgument, "--report=", 9) == 0) {
                options._ReportFilePath = currentArgument + 9;
            }
        }

        return !options._NavMeshFilePath.empty() && !options._CasesFilePath.empty();
    }

    static bool TryReadVector3(const Json::Value& value, Vector3& point) {
        if (!value.isArray() || value.size() != 3) {
            return false;
        }

        point.x = value[0].asFloat();
        point.y = value[1].asFloat();
        point.z = value[2].asFloat();
        return true;
    }

    static Json::Value ToJsonVector3(const Vector3& point) {
        Json::Value value(Json::arrayValue);
        value.append(point.x);
        value.append(point.y);
        value.append(point.z);
        return value;
    }

    static float Distance(const Vector3& a, const Vector3& b) {
        return (a - b).Length();
    }

    static float CalculatePathLength(const std::vector<Vector3>& pathPoints) {
        float length = 0.0f;
        for (int index = 1; index < static_cast<int>(pathPoints.size()); index++) {
            length += Distance(pathPoints[index - 1], pathPoints[index]);
        }

        return length;
    }

    static void PushFailure(Json::Value& caseResult, const std::string& reason) {
        caseResult["passed"] = false;
        caseResult["reasons"].append(reason);
    }

    static bool ValidateNearestCase(Api::iNavMesh* const navMesh, const Json::Value& caseRoot, Json::Value& caseResult) {
        const std::string caseName = caseRoot["name"].asString();
        caseResult["name"] = caseName;
        caseResult["type"] = "nearest";
        caseResult["passed"] = true;
        caseResult["reasons"] = Json::Value(Json::arrayValue);

        Vector3 point;
        if (!TryReadVector3(caseRoot["point"], point)) {
            PushFailure(caseResult, "point invalid");
            return false;
        }

        const float searchRadius = caseRoot["searchRadius"].asFloat();
        const float agentRadius = caseRoot["agentRadius"].asFloat();
        const float agentHeight = caseRoot["agentHeight"].asFloat();
        const bool expectSuccess = caseRoot["expectSuccess"].asBool();
        const float tolerance = caseRoot.isMember("positionTolerance") ? caseRoot["positionTolerance"].asFloat() : 0.25f;

        Vector3 resultPoint;
        const bool actualSuccess = navMesh->FindNearestWalkablePoint(point, searchRadius, agentRadius, agentHeight, resultPoint);

        caseResult["input"]["point"] = ToJsonVector3(point);
        caseResult["input"]["searchRadius"] = searchRadius;
        caseResult["input"]["agentRadius"] = agentRadius;
        caseResult["input"]["agentHeight"] = agentHeight;
        caseResult["expect"]["success"] = expectSuccess;
        caseResult["actual"]["success"] = actualSuccess;

        if (actualSuccess) {
            caseResult["actual"]["point"] = ToJsonVector3(resultPoint);
        }

        if (actualSuccess != expectSuccess) {
            PushFailure(caseResult, "success mismatch");
        }

        if (actualSuccess && caseRoot.isMember("expectPoint")) {
            Vector3 expectPoint;
            if (!TryReadVector3(caseRoot["expectPoint"], expectPoint)) {
                PushFailure(caseResult, "expectPoint invalid");
            }
            else {
                const float errorDistance = Distance(resultPoint, expectPoint);
                caseResult["expect"]["point"] = ToJsonVector3(expectPoint);
                caseResult["actual"]["pointError"] = errorDistance;
                if (errorDistance > tolerance) {
                    PushFailure(caseResult, "point error exceeds tolerance");
                }
            }
        }

        return caseResult["passed"].asBool();
    }

    static bool ValidatePathCase(Api::iNavMesh* const navMesh, const Json::Value& caseRoot, Json::Value& caseResult) {
        const std::string caseName = caseRoot["name"].asString();
        caseResult["name"] = caseName;
        caseResult["type"] = "path";
        caseResult["passed"] = true;
        caseResult["reasons"] = Json::Value(Json::arrayValue);

        Vector3 begin;
        Vector3 end;
        if (!TryReadVector3(caseRoot["begin"], begin)) {
            PushFailure(caseResult, "begin invalid");
            return false;
        }

        if (!TryReadVector3(caseRoot["end"], end)) {
            PushFailure(caseResult, "end invalid");
            return false;
        }

        const float agentRadius = caseRoot["agentRadius"].asFloat();
        const float agentHeight = caseRoot["agentHeight"].asFloat();
        const bool expectSuccess = caseRoot["expectSuccess"].asBool();
        const float tolerance = caseRoot.isMember("positionTolerance") ? caseRoot["positionTolerance"].asFloat() : 0.25f;

        std::vector<Vector3> pathPoints;
        const bool actualSuccess = navMesh->FindPath(begin, end, agentRadius, agentHeight, pathPoints);

        caseResult["input"]["begin"] = ToJsonVector3(begin);
        caseResult["input"]["end"] = ToJsonVector3(end);
        caseResult["input"]["agentRadius"] = agentRadius;
        caseResult["input"]["agentHeight"] = agentHeight;
        caseResult["expect"]["success"] = expectSuccess;
        caseResult["actual"]["success"] = actualSuccess;
        caseResult["actual"]["pointCount"] = static_cast<int>(pathPoints.size());

        if (actualSuccess) {
            Json::Value pathArray(Json::arrayValue);
            for (const Vector3& point : pathPoints) {
                pathArray.append(ToJsonVector3(point));
            }
            caseResult["actual"]["points"] = pathArray;
            caseResult["actual"]["pathLength"] = CalculatePathLength(pathPoints);
        }

        if (actualSuccess != expectSuccess) {
            PushFailure(caseResult, "success mismatch");
        }

        if (actualSuccess && caseRoot.isMember("expectPointCountMin") && static_cast<int>(pathPoints.size()) < caseRoot["expectPointCountMin"].asInt()) {
            PushFailure(caseResult, "point count below minimum");
        }

        if (actualSuccess && caseRoot.isMember("expectPointCountMax") && static_cast<int>(pathPoints.size()) > caseRoot["expectPointCountMax"].asInt()) {
            PushFailure(caseResult, "point count above maximum");
        }

        if (actualSuccess && caseRoot.isMember("expectPathLengthMin") && caseResult["actual"]["pathLength"].asFloat() < caseRoot["expectPathLengthMin"].asFloat()) {
            PushFailure(caseResult, "path length below minimum");
        }

        if (actualSuccess && caseRoot.isMember("expectPathLengthMax") && caseResult["actual"]["pathLength"].asFloat() > caseRoot["expectPathLengthMax"].asFloat()) {
            PushFailure(caseResult, "path length above maximum");
        }

        if (actualSuccess && caseRoot.isMember("expectEndPoint")) {
            Vector3 expectEndPoint;
            if (!TryReadVector3(caseRoot["expectEndPoint"], expectEndPoint)) {
                PushFailure(caseResult, "expectEndPoint invalid");
            }
            else if (!pathPoints.empty()) {
                const float errorDistance = Distance(pathPoints.back(), expectEndPoint);
                caseResult["expect"]["endPoint"] = ToJsonVector3(expectEndPoint);
                caseResult["actual"]["endPointError"] = errorDistance;
                if (errorDistance > tolerance) {
                    PushFailure(caseResult, "end point error exceeds tolerance");
                }
            }
        }

        if (actualSuccess) {
            for (int index = 1; index < static_cast<int>(pathPoints.size()); index++) {
                Vector3 hitPoint;
                if (navMesh->Raycast(pathPoints[index - 1], pathPoints[index], agentRadius, agentHeight, hitPoint)) {
                    PushFailure(caseResult, "path segment blocked");
                    caseResult["actual"]["blockedSegmentIndex"] = index - 1;
                    caseResult["actual"]["blockedPoint"] = ToJsonVector3(hitPoint);
                    break;
                }
            }
        }

        return caseResult["passed"].asBool();
    }

    static bool ValidateRaycastCase(Api::iNavMesh* const navMesh, const Json::Value& caseRoot, Json::Value& caseResult) {
        const std::string caseName = caseRoot["name"].asString();
        caseResult["name"] = caseName;
        caseResult["type"] = "raycast";
        caseResult["passed"] = true;
        caseResult["reasons"] = Json::Value(Json::arrayValue);

        Vector3 begin;
        Vector3 end;
        if (!TryReadVector3(caseRoot["begin"], begin)) {
            PushFailure(caseResult, "begin invalid");
            return false;
        }

        if (!TryReadVector3(caseRoot["end"], end)) {
            PushFailure(caseResult, "end invalid");
            return false;
        }

        const float agentRadius = caseRoot["agentRadius"].asFloat();
        const float agentHeight = caseRoot["agentHeight"].asFloat();
        const bool expectHit = caseRoot["expectHit"].asBool();
        const float tolerance = caseRoot.isMember("positionTolerance") ? caseRoot["positionTolerance"].asFloat() : 0.25f;

        Vector3 hitPoint;
        const bool actualHit = navMesh->Raycast(begin, end, agentRadius, agentHeight, hitPoint);

        caseResult["input"]["begin"] = ToJsonVector3(begin);
        caseResult["input"]["end"] = ToJsonVector3(end);
        caseResult["input"]["agentRadius"] = agentRadius;
        caseResult["input"]["agentHeight"] = agentHeight;
        caseResult["expect"]["hit"] = expectHit;
        caseResult["actual"]["hit"] = actualHit;

        if (actualHit) {
            caseResult["actual"]["hitPoint"] = ToJsonVector3(hitPoint);
        }

        if (actualHit != expectHit) {
            PushFailure(caseResult, "hit mismatch");
        }

        if (actualHit && caseRoot.isMember("expectHitPoint")) {
            Vector3 expectHitPoint;
            if (!TryReadVector3(caseRoot["expectHitPoint"], expectHitPoint)) {
                PushFailure(caseResult, "expectHitPoint invalid");
            }
            else {
                const float errorDistance = Distance(hitPoint, expectHitPoint);
                caseResult["expect"]["hitPoint"] = ToJsonVector3(expectHitPoint);
                caseResult["actual"]["hitPointError"] = errorDistance;
                if (errorDistance > tolerance) {
                    PushFailure(caseResult, "hit point error exceeds tolerance");
                }
            }
        }

        return caseResult["passed"].asBool();
    }

    static void CountCaseResult(const bool passed, ValidationSummary& summary) {
        summary._TotalCount++;
        if (passed) {
            summary._PassedCount++;
        }
        else {
            summary._FailedCount++;
        }
    }

    static Json::Value BuildSummaryJson(const ValidationSummary& summary) {
        Json::Value value(Json::objectValue);
        value["total"] = summary._TotalCount;
        value["passed"] = summary._PassedCount;
        value["failed"] = summary._FailedCount;
        return value;
    }
}

int main(int argc, const char** argv) {
    CommandLineOptions options;
    if (!ParseCommandLineOptions(argc, argv, options)) {
        printf("Usage: NavMeshValidator --navmesh=<file> --cases=<file> [--report=<file>]\n");
        return 1;
    }

    Api::iNavMesh* navMesh = NavMesh::Create(options._NavMeshFilePath);
    if (!navMesh) {
        printf("Load navmesh failed: %s\n", options._NavMeshFilePath.c_str());
        return 2;
    }

    Json::Value casesRoot;
    std::string loadError;
    if (!LoadJsonFile(options._CasesFilePath, casesRoot, loadError)) {
        printf("Load cases failed: %s\n", loadError.c_str());
        navMesh->Release();
        return 3;
    }

    Json::Value reportRoot(Json::objectValue);
    reportRoot["navmeshFilePath"] = options._NavMeshFilePath;
    reportRoot["casesFilePath"] = options._CasesFilePath;

    ValidationSummary summary;

    Json::Value nearestResults(Json::arrayValue);
    for (const Json::Value& nearestCase : casesRoot["nearest"]) {
        Json::Value caseResult;
        const bool passed = ValidateNearestCase(navMesh, nearestCase, caseResult);
        CountCaseResult(passed, summary);
        nearestResults.append(caseResult);
    }
    reportRoot["nearest"] = nearestResults;

    Json::Value pathResults(Json::arrayValue);
    for (const Json::Value& pathCase : casesRoot["path"]) {
        Json::Value caseResult;
        const bool passed = ValidatePathCase(navMesh, pathCase, caseResult);
        CountCaseResult(passed, summary);
        pathResults.append(caseResult);
    }
    reportRoot["path"] = pathResults;

    Json::Value raycastResults(Json::arrayValue);
    for (const Json::Value& raycastCase : casesRoot["raycast"]) {
        Json::Value caseResult;
        const bool passed = ValidateRaycastCase(navMesh, raycastCase, caseResult);
        CountCaseResult(passed, summary);
        raycastResults.append(caseResult);
    }
    reportRoot["raycast"] = raycastResults;

    reportRoot["summary"] = BuildSummaryJson(summary);

    if (!options._ReportFilePath.empty()) {
        std::string saveError;
        if (!SaveJsonFile(options._ReportFilePath, reportRoot, saveError)) {
            printf("Save report failed: %s\n", saveError.c_str());
            navMesh->Release();
            return 4;
        }
    }

    printf("NavMeshValidator summary: total=%d passed=%d failed=%d\n", summary._TotalCount, summary._PassedCount, summary._FailedCount);
    navMesh->Release();
    return summary._FailedCount == 0 ? 0 : 5;
}
