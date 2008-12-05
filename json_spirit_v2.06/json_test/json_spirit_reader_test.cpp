/* Copyright (c) 2007-2008 John W Wilkinson

   This source code can be used for any purpose as long as
   this comment is retained. */

// json spirit version 2.06

#include "json_spirit_reader_test.h"
#include "json_spirit_reader.h"
#include "json_spirit_value.h" 
#include "json_spirit_writer.h" 
#include "utils_test.h"

#include <sstream>
#include <boost/assign/list_of.hpp>
#include <boost/timer.hpp>
#include <boost/lexical_cast.hpp>

using namespace json_spirit;
using namespace std;
using namespace boost;
using namespace boost::assign;

namespace
{
    template< class Value_t >
    struct Test_runner
    {
        typedef typename Value_t::String_type     String_t;
        typedef typename Value_t::Object          Object_t;
        typedef typename Value_t::Array           Array_t;
        typedef typename String_t::value_type     Char_t;
        typedef typename String_t::const_iterator Iter_t;
        typedef Pair_impl< String_t >             Pair_t;
        typedef std::basic_istream< Char_t >      Istream_t;

        String_t to_str( const char* c_str )
        {
            return ::to_str< String_t >( c_str );
        }

        Pair_t make_pair( const char* c_name, const char* c_value )
        {
            return Pair_t( to_str( c_name ), to_str( c_value ) );
        }

        Pair_t p1;
        Pair_t p2;
        Pair_t p3;

        Test_runner()
        :   p1( make_pair("name 1", "value 1") )
        ,   p2( make_pair("name 2", "value 2") )
        ,   p3( make_pair("name 3", "value 3") )
        {
        }

        void check_eq( const Object_t& obj_1, const Object_t& obj_2 )
        {
            const typename Object_t::size_type size( obj_1.size() );

            assert_eq( size, obj_2.size() );

            for( typename Object_t::size_type i = 0; i < size; ++i )
            {
                assert_eq( obj_1[i], obj_2[i] ); 
            }
        }

        void test_syntax( const char* c_str, bool expected_result = true )
        {
            Value_t value;

            const bool success = read( to_str( c_str ), value );

            assert_eq( success, expected_result );
        }

        template< typename Int >
        void test_syntax( Int min_int, Int max_int )
        {
            ostringstream os;

            os << "[" << min_int << "," << max_int << "]";

            test_syntax( os.str().c_str() );
        }

        void test_syntax()
        {
			test_syntax( "{'contents': '물론 뒷북입니다만 ^^ 이글루스가 드디어 만 18세 이상 봉인이 14세 이상 봉인으로 변경되었습니다.\r\n정책 변경 후에 주변에 만 14세~17세 이글루 유저가 좀 눈에 띄게 늘어났는지요? \r\n아직은 이글루스 운영자와 유저들의 기대와 설레발과는 달리 그렇게 큰 유저수 증가와 같은 변화는 감지할 수가 없는 것 같습니다.\r\n\r\n<span style=\"FONT-SIZE: 130%\"><strong>얏호, 얼음집 만들고 상품 받자!<\/strong><\/span> 이벤트 같은걸 띄워서 네이트 중앙에 크게 올려보면 좀 몰려오지 않을지?\r\n그렇죠. 어쨌거나 대세적으로는 이글루스는 듣보잡 마이너 블로그 서비스인거죠.\r\n저라면 차라리 네이트 통이나 엠파스 블로그 같은걸 만들겠어요. (?)\r\n\r\n솔직히 저 개인적으로는 만 14세 이상 가입 가능보다는 네이트 계정으로도 이글루스를 만들 수 있게 됐어~ 가 더 큰 변화가 아닐까 싶습니다.\r\n\r\n물론 나 네이트 싫어, 이글루스만 쓸거야 하는 분들을 위해서, 이글루스만 가입할 수 있도록 하는 기능도 여전히 살아 있는 것을 볼 수 있긴 합니다. (그런데 보통의 포털 서비스들은 하위 서비스를 이용하고자 할 경우 상위 서비스를 이용하도록 강제하거나 권장하는데 이글루스 가입 시에는 네이트를 언급조차 하지 않는다는게 재미있습니다)\r\n\r\n그...그런데. 가입폼을 보니...\r\n[#IMAGE|a0004222_4924b0ba20c4d.png|pds\/200811\/20\/22\/|mid|979|700|pds12#]<div align=\"center\">나이 제한 유지 여부를 놓고 끝까지 싸우던 이글루스 운영자들의 흔적.png<\/div>\r\n\r\n저건 그렇다치고 가입 절차가 리뉴얼 전보다 까다로워졌음은 분명하게 확인됩니다. 실명 확인\/I-PIN 확인은 인터넷 서비스라면 넣어두는 것이 폼나니까 그렇다 치는데, 새롭게 추가된 개인 인증 이라는 부분이 눈에 뜨이지요. 핸드폰, 공인인증서, 신분증 스캔해서 보내기(...)까지 마련해두었습니다.\r\n사실 이런 귀찮은 가입 단계가 멋지다면 멋질수도 있고 번거로워졌다면 번거로워진 것이긴 한데...\r\n어쨌든 이글루스에 주민등록번호를 훔쳐다 가입하는 것을 봉쇄해보겠다는 강력한 의지로 보이긴 합니다.\r\n휴대폰 인증 번호까지 받아가며 본인 인증을 해야 하는 새로운 체제에 대해 싫어하실 분들은 싫어하시겠지만, 이러한 조용한 변경에 대해 환영하시는 분들도 분명히 적지 않으시리라 생각하구요. (전 아무래도 괜찮습니다만-.-) 아아, 이제는 피닉스의 화려한 부활을 다시는 볼 수 없게 되는건가!\r\n\r\n그런데 네이트 회원이 이글루스 계정을 만드는 경우는 어떨까요?\r\n회원 가입 버튼을 통해서 계정 만들기는 불가능하고, (현시점에서는 회원 가입 버튼을 눌렀을 경우 네이트 계정을 갖고 계신다면 이쪽으로~ 같은 안내가 없죠) 그냥 생각없이 네이트 아이디와 패스워드로 로그인하면 아직 이글루를 만들지 않았으니 만들고 쓰라는 안내 문구를 보게 됩니다.\r\n[#IMAGE|a0004222_4924b1e819542.png|pds\/200811\/20\/22\/|mid|979|700|pds10#]<div align=\"center\">이것이 네이트 회원의 혜택.png<\/div>\r\n닉네임과 이글루 주소만 넣고 이용 약관 동의 버튼만 클릭하면 바로 개설됩니다.\r\n개인 인증? 공인인증서? 신분증? 그게 뭐지...? 뭐였지? 뭐더라?\r\n\r\n아, 하긴 네이트 가입할 때 개인 인증 별도로 할테니까 상관 없겠구나.\r\n[#IMAGE|a0004222_4924b34133769.png|pds\/200811\/20\/22\/|mid|982|988|pds10#]<div align=\"center\">네이트 회원 가입 페이지.png<\/div>\r\n네이트 가입페이지 구경갔습니다.\r\n개인 인증? ... 없습니다.\r\n뭐... 뭐져? -_-;;;\r\n\r\n일반적으로 주민등록번호를 도용하는 경우, 나름의 안전(?)을 위해 부모님이나 할머니, 컴퓨터 전혀 안 쓸거 같은 사람 쪽의 주민등록번호를 도용하는 경우가 많다는 것을 생각해보면, 이글루를 개설하고자 의도적으로 주민등록번호를 도용하는 경우, 모처럼 신설된 본인 인증 부분은 네이트 계정을 만들고 이글루를 개설하는 것으로 간단하게 회피할 수 있게 되어버립니다.\r\n\r\n뭐 현 시점에서는 네이트 계정 하나로는 이글루를 하나 밖에 가질 수 없기 때문에, 어차피 추가 개설할 일이 생기면 매번 이글루 계정을 만들면서 개인 인증을 받아야하는 벽이 있기는 합니다만, 이럴거면 개인 인증을 추가하지 않으셨다면 더 좋았을 걸, 이라고 해야할지 아니면 개인 인증 없던 시절에 미리 가입해둔 나는야 승리자 와하하핫, 이라고 해야될지.\r\n\r\n뭐 어쨌든 이글루 만들 사람 중에 네이트 계정 안갖고 있는 사람 비율이 몇 퍼센트나 될까 싶기도 하지만, 본인 인증 자체에 대해 들고 일어날 생각도 그다지 없지만, 변경된 부분을 살펴보다보니 아무튼 뭐 저런 아이러니한 부분이 있더라는 거...죠.', 'moreSubject':'', 'moreContents':''}");

			return;

            test_syntax( "{}" );
            test_syntax( "{ }" );
            test_syntax( "{\"\":\"\"}" );
            test_syntax( "{\"test\":\"123\"}" );
            test_syntax( "{\"test\" : \"123\"}" );
            test_syntax( "{\"testing testing testing\":\"123\"}" );
            test_syntax( "{\"\":\"abc\"}" );
            test_syntax( "{\"abc\":\"\"}" );
            test_syntax( "{\"\":\"\"}" );
            test_syntax( "{\"test\":true}" );
            test_syntax( "{\"test\":false}" );
            test_syntax( "{\"test\":null}" );
            test_syntax( "{\"test1\":\"123\",\"test2\":\"456\"}" );
            test_syntax( "{\"test1\":\"123\",\"test2\":\"456\",\"test3\":\"789\"}" );
            test_syntax( "{\"test1\":{\"test2\":\"123\",\"test3\":\"456\"}}" );
            test_syntax( "{\"test1\":{\"test2\":{\"test3\":\"456\"}}}" );
            test_syntax( "{\"test1\":[\"a\",\"bb\",\"cc\"]}" );
            test_syntax( "{\"test1\":[true,false,null]}" );
            test_syntax( "{\"test1\":[true,\"abc\",{\"a\":\"b\"},{\"d\":false},null]}" );
            test_syntax( "{\"test1\":[1,2,-3]}" );
            test_syntax( "{\"test1\":[1.1,2e4,-1.234e-34]}" );
            test_syntax( "{\n"
                          "\t\"test1\":\n"
                          "\t\t{\n"
                          "\t\t\t\"test2\":\"123\",\n"
                          "\t\t\t\"test3\":\"456\"\n"
                          "\t\t}\n"
                          "}\n" );
            test_syntax( "[]" );
            test_syntax( "[ ]" );
            test_syntax( "[1,2,3]" );
            test_syntax( "[ 1, -2, 3]" );
            test_syntax( "[ 1.2, -2e6, -3e-6 ]" );
            test_syntax( "[ 1.2, \"str\", -3e-6, { \"field\" : \"data\" } ]" );

            test_syntax( INT_MIN, INT_MAX );
            test_syntax( LLONG_MIN, LLONG_MAX );
            test_syntax( "[1 2 3]", false );
        }

        Value_t read_cstr( const char* c_str )
        {
            Value_t value;

            read( to_str( c_str ), value );

            return value;
        }

        bool read_cstr( const char* c_str, Value_t& value )
        {
            return read( to_str( c_str ), value );
        }

        void check_read( const char* c_str, Value_t& value, bool expected_result = true )
        {
            assert_eq( read_cstr( c_str, value ), expected_result );
        }

        void check_reading( const char* c_str )
        {
            Value_t value;

            String_t in_s( to_str( c_str ) );

            const bool success = read( in_s, value );

            assert_eq( success, true );

            const String_t result = write_formatted( value ); 

//            cout << in_s.c_str() << endl << result.c_str() << endl ;

            assert_eq( in_s, result );
        }

        template< typename Int >
        void check_reading( Int min_int, Int max_int )
        {
            ostringstream os;

            os << "[\n"
                   "    " << min_int << ",\n"
                   "    " << max_int << "\n"
                   "]";

            check_reading( os.str().c_str() );
        }

        void test_reading()
        {
            check_reading( "{\n}" );

            Value_t value;

            check_read( "{\n"
                        "    \"name 1\" : \"value 1\"\n"
                        "}", value );

            check_eq( value.get_obj(), list_of( p1 ) );

            check_read( "{\"name 1\":\"value 1\",\"name 2\":\"value 2\"}", value );

            check_eq( value.get_obj(), list_of( p1 )( p2 ) );

            check_read( "{\n"
                        "    \"name 1\" : \"value 1\",\n"
                        "    \"name 2\" : \"value 2\",\n"
                        "    \"name 3\" : \"value 3\"\n"
                        "}", value );

            check_eq( value.get_obj(), list_of( p1 )( p2 )( p3 ) );

            check_read( "{\n"
                        "    \"\" : \"value\",\n"
                        "    \"name\" : \"\"\n"
                        "}", value );

            check_eq( value.get_obj(), list_of( make_pair( "", "value" ) )( make_pair( "name", "" ) ) );

            check_reading( "{\n"
                            "    \"name 1\" : \"value 1\",\n"
                            "    \"name 2\" : {\n"
                            "        \"name 3\" : \"value 3\",\n"
                            "        \"name_4\" : \"value_4\"\n"
                            "    }\n"
                            "}" );

            check_reading( "{\n"
                            "    \"name 1\" : \"value 1\",\n"
                            "    \"name 2\" : {\n"
                            "        \"name 3\" : \"value 3\",\n"
                            "        \"name_4\" : \"value_4\",\n"
                            "        \"name_5\" : {\n"
                            "            \"name_6\" : \"value_6\",\n"
                            "            \"name_7\" : \"value_7\"\n"
                            "        }\n"
                            "    }\n"
                            "}" );

            check_reading( "{\n"
                            "    \"name 1\" : \"value 1\",\n"
                            "    \"name 2\" : {\n"
                            "        \"name 3\" : \"value 3\",\n"
                            "        \"name_4\" : {\n"
                            "            \"name_5\" : \"value_5\",\n"
                            "            \"name_6\" : \"value_6\"\n"
                            "        },\n"
                            "        \"name_7\" : \"value_7\"\n"
                            "    }\n"
                            "}" );

            check_reading( "{\n"
                            "    \"name 1\" : \"value 1\",\n"
                            "    \"name 2\" : {\n"
                            "        \"name 3\" : \"value 3\",\n"
                            "        \"name_4\" : {\n"
                            "            \"name_5\" : \"value_5\",\n"
                            "            \"name_6\" : \"value_6\"\n"
                            "        },\n"
                            "        \"name_7\" : \"value_7\"\n"
                            "    },\n"
                            "    \"name_8\" : \"value_8\",\n"
                            "    \"name_9\" : {\n"
                            "        \"name_10\" : \"value_10\"\n"
                            "    }\n"
                            "}" );

            check_reading( "{\n"
                            "    \"name 1\" : {\n"
                            "        \"name 2\" : {\n"
                            "            \"name 3\" : {\n"
                            "                \"name_4\" : {\n"
                            "                    \"name_5\" : \"value\"\n"
                            "                }\n"
                            "            }\n"
                            "        }\n"
                            "    }\n"
                            "}" );

            check_reading( "{\n"
                            "    \"name 1\" : \"value 1\",\n"
                            "    \"name 2\" : true,\n"
                            "    \"name 3\" : false,\n"
                            "    \"name_4\" : \"value_4\",\n"
                            "    \"name_5\" : true\n"
                            "}" );

            check_reading( "{\n"
                            "    \"name 1\" : \"value 1\",\n"
                            "    \"name 2\" : null,\n"
                            "    \"name 3\" : \"value 3\",\n"
                            "    \"name_4\" : null\n"
                            "}" );

            check_reading( "{\n"
                            "    \"name 1\" : \"value 1\",\n"
                            "    \"name 2\" : 123,\n"
                            "    \"name 3\" : \"value 3\",\n"
                            "    \"name_4\" : -567\n"
                            "}" );

            check_reading( "{\n"
                            "    \"name 1\" : \"value 1\",\n"
                            "    \"name 2\" : 1.200000000000000,\n"
                            "    \"name 3\" : \"value 3\",\n"
                            "    \"name_4\" : 1.234567890123456e+125,\n"
                            "    \"name_5\" : -1.234000000000000e-123,\n"
                            "    \"name_6\" : 1.000000000000000e-123,\n"
                            "    \"name_7\" : 1234567890.123456\n"
                            "}" );

            check_reading( "[\n]" );

            check_reading( "[\n"
                           "    1\n"
                           "]" );

            check_reading( "[\n"
                           "    1,\n"
                           "    1.200000000000000,\n"
                           "    \"john\",\n"
                           "    true,\n"
                           "    false,\n"
                           "    null\n"
                           "]" );

            check_reading( "[\n"
                           "    1,\n"
                           "    [\n"
                           "        2,\n"
                           "        3\n"
                           "    ]\n"
                           "]" );

            check_reading( "[\n"
                           "    1,\n"
                           "    [\n"
                           "        2,\n"
                           "        3\n"
                           "    ],\n"
                           "    [\n"
                           "        4,\n"
                           "        [\n"
                           "            5,\n"
                           "            6,\n"
                           "            7\n"
                           "        ]\n"
                           "    ]\n"
                           "]" );

            check_reading( "[\n"
                           "    {\n"
                           "        \"name\" : \"value\"\n"
                           "    }\n"
                           "]" );

            check_reading( "{\n"
                           "    \"name\" : [\n"
                           "        1\n"
                           "    ]\n"
                           "}" );

            check_reading( "[\n"
                           "    {\n"
                           "        \"name 1\" : \"value\",\n"
                           "        \"name 2\" : [\n"
                           "            1,\n"
                           "            2,\n"
                           "            3\n"
                           "        ]\n"
                           "    }\n"
                           "]" );

            check_reading( "{\n"
                           "    \"name 1\" : [\n"
                           "        1,\n"
                           "        {\n"
                           "            \"name 2\" : \"value 2\"\n"
                           "        }\n"
                           "    ]\n"
                           "}" );

            check_reading( "[\n"
                           "    {\n"
                           "        \"name 1\" : \"value 1\",\n"
                           "        \"name 2\" : [\n"
                           "            1,\n"
                           "            2,\n"
                           "            {\n"
                           "                \"name 3\" : \"value 3\"\n"
                           "            }\n"
                           "        ]\n"
                           "    }\n"
                           "]" );

            check_reading( "{\n"
                           "    \"name 1\" : [\n"
                           "        1,\n"
                           "        {\n"
                           "            \"name 2\" : [\n"
                           "                1,\n"
                           "                2,\n"
                           "                3\n"
                           "            ]\n"
                           "        }\n"
                           "    ]\n"
                           "}" );

            check_reading( INT_MIN, INT_MAX );
            check_reading( LLONG_MIN, LLONG_MAX );
        }

        void test_from_stream()
        {
            Value_t value;

            String_t in_s( to_str( "[1,2]" ) );

            basic_istringstream< Char_t > is( in_s );

            const bool success = read( is, value );

            assert_eq( success, true );

            assert_eq( in_s, write( value ) );
       }

        void test_escape_chars( const char* json_str, const char* c_str )
        {
            Value_t value;

            string s( string( "{\"" ) + json_str + "\" : \"" + json_str + "\"} " );

            check_read( s.c_str(), value );

            const Pair_t& pair( value.get_obj()[0] );

            assert_eq( pair.name_,  to_str( c_str ) );
            assert_eq( pair.value_, to_str( c_str ) );
        }

        void test_escape_chars()
        {
            test_escape_chars( "\\t", "\t");
            test_escape_chars( "a\\t", "a\t" );
            test_escape_chars( "\\tb", "\tb" );
            test_escape_chars( "a\\tb", "a\tb" );
            test_escape_chars( "a\\tb", "a\tb" );
            test_escape_chars( "a123\\tb", "a123\tb" );
            test_escape_chars( "\\t\\n\\\\", "\t\n\\" );
            test_escape_chars( "\\/\\r\\b\\f\\\"", "/\r\b\f\"" );
            test_escape_chars( "\\h\\j\\k", "" ); // invalid esc chars
            test_escape_chars( "\\x61\\x62\\x63", "abc" );
            test_escape_chars( "a\\x62c", "abc" );
            test_escape_chars( "\\x01\\x02\\x7F", "\x01\x02\x7F" ); // NB x7F is the greatest char spirit will parse
            test_escape_chars( "\\u0061\\u0062\\u0063", "abc" );
        }

        void check_is_null( const char* c_str  )
        {
            assert_eq( read_cstr( c_str ).type(), null_type ); 
        }

        template< typename T >
        void check_value( const char* c_str, const T& expected_value )
        {
            const Value_t v( read_cstr( c_str ) );
            
            assert_eq( v.template get_value< T >(), expected_value ); 
        }

        void test_values()
        {
            check_value( "1",        1 );
            check_value( "1.5",      1.5 );
            check_value( "\"Test\"", to_str( "Test" ) );
            check_value( "true",     true );
            check_value( "false",    false );
            check_is_null( "null" );
        }

        void check_read_fails( const char* c_str )
        {
            Value_t value;

            check_read( c_str, value, false );
        }

        void test_error_cases()
        {
            check_read_fails( "[\"1\\\\\",\"2\\\"]" );
            check_read_fails( "." );
            check_read_fails( "1 1" );
            check_read_fails( "\"\"\"" );
            check_read_fails( "'1'" );
            check_read_fails( "{1 2}" );
            check_read_fails( "1.263Q" );
            check_read_fails( "1.26 3" );
            check_read_fails( "'" );
            check_read_fails( "[1 2]" );
        }

        void run_tests()
        {
            test_syntax();
            test_reading();
            test_from_stream();
            test_escape_chars();
            test_values();
            test_error_cases();
        }
    };

#ifndef BOOST_NO_STD_WSTRING
    void test_wide_esc_u()
    {
        wValue value;

        const bool success = read( L"[\"\\uABCD\"]", value );

        assert( success );

        const wstring s( value.get_array()[0].get_str() );

        assert_eq( s.length(), static_cast< wstring::size_type >( 1u ) );
        assert_eq( s[0], 0xABCD );
    }
#endif

    void test_extended_ascii( const string& s )
    {
        Value value;

        const bool success = read( "[\"" + s + "\"]", value );

        assert_eq( success, true );

        assert_eq( value.get_array()[0].get_str(), "隘廻" );
    }

    void test_extended_ascii()
    {
        test_extended_ascii( "\\u00E4\\u00F6\\u00FC\\u00DF" );
        test_extended_ascii( "隘廻" );
    }
}

void json_spirit::test_reader()
{
    Test_runner< Value >().run_tests();

#ifndef BOOST_NO_STD_WSTRING
    Test_runner< wValue >().run_tests();
    test_wide_esc_u();
#endif

    test_extended_ascii();

    //Object obj;

    //for( int i = 0; i < 100000; ++i )
    //{
    //    obj.push_back( Pair( "\x01test\x7F", lexical_cast< string >( i ) ) );
    //}

    //const string s = write( obj );

    //Value value;

    //timer t;

    //read( s, value );

    //cout << t.elapsed() << endl;

    //cout << "obj size " << value.get_obj().size();
}
