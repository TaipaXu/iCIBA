#include "./network.hpp"
#include <iostream>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>

std::variant<Model::WordResult, Model::SentenceResult> Network::translate(const std::string &text)
{
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace net = boost::asio;
    namespace json = boost::json;
    using tcp = net::ip::tcp;

    try
    {
        net::io_context ioContext;

        tcp::resolver resolver(ioContext);
        auto const results = resolver.resolve("dict-pc.iciba.com", "80");

        tcp::socket socket(ioContext);
        net::connect(socket, results.begin(), results.end());

        const std::map<std::string, std::string> params = {
            {"client", "5"},
            {"type", "1"},
            {"timestamp", "1557025419"},
            {"uuid", "CB5082D19C82440F836DE3AED8E5FEB5"},
            {"c", "word"},
            {"m", "index"},
            {"v", "2016.3.3.0333"},
            {"sign", "cf2decaa9965af29"},
            {"list", "1"},
            {"word", text},
        };
        const std::string queryString = makeQueryString(params);

        const std::string target = "/interface/index.php?" + queryString;
        http::request<http::string_body> request(http::verb::get, target, 11);
        request.set(http::field::host, "dict-pc.iciba.com");

        http::write(socket, request);

        beast::flat_buffer buffer;
        http::response<http::dynamic_body> response;
        http::read(socket, buffer, response);

        const std::string body = beast::buffers_to_string(response.body().data());
        boost::system::error_code jsonErrorCode;
        const json::value jsonValue = json::parse(body, jsonErrorCode);
        if (jsonErrorCode)
        {
            throw std::runtime_error("Failed to parse JSON");
        }

        json::object obj = jsonValue.as_object();
        json::object messageObject = obj["message"].as_object();
        json::object baseInfo = messageObject["baesInfo"].as_object();

        const int type = baseInfo["translate_type"].as_int64();
        if (type == 1)
        {
            Model::WordResult wordResult;
            wordResult.word = baseInfo["word_name"].as_string().c_str();

            const json::array symbolsArray = baseInfo["symbols"].as_array();
            json::object jsonSymbol = symbolsArray[0].as_object();
            json::array jsonParts = jsonSymbol["parts"].as_array();

            for (const auto &part : jsonParts)
            {
                json::object partObj = part.as_object();
                std::vector<std::string> meanings;
                for (const auto &mean : partObj["means"].as_array())
                {
                    meanings.push_back(mean.as_string().c_str());
                }
                wordResult.meanings.push_back({partObj["part"].as_string().c_str(), meanings});
            }

            wordResult.pronunciation = {
                jsonSymbol["ph_en"].as_string().c_str(),
                jsonSymbol["ph_am"].as_string().c_str(),
                jsonSymbol["ph_other"].as_string().c_str()};

            return wordResult;
        }
        else if (type == 2)
        {
            Model::SentenceResult sentenceResult;
            sentenceResult.sentence = messageObject["word_name"].as_string().c_str();
            sentenceResult.translation = baseInfo["translate_result"].as_string().c_str();

            return sentenceResult;
        }
        else
        {
            throw std::runtime_error("Failed to translate");
        }

        beast::error_code errorCode;
        socket.shutdown(tcp::socket::shutdown_both, errorCode);
        if (errorCode && errorCode != beast::errc::not_connected)
        {
            throw std::runtime_error("network error");
        }
    }
    catch (std::exception const &e)
    {
        throw;
    }

    throw std::runtime_error("Failed to translate");
}

std::string Network::makeQueryString(const std::map<std::string, std::string> &params) const
{
    std::string query;
    for (const auto &p : params)
    {
        if (!query.empty())
        {
            query += "&";
        }
        query += p.first + "=" + p.second;
    }
    return query;
}
