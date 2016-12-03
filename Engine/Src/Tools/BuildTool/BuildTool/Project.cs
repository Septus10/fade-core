using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

namespace Fade
{
    namespace BuildTool
    {
        /* Enum of platforms 
         * Only 64 bit is supported for this engine
         */
        public enum EPlatform
        {
            /* Windows 64 bit */
            Windows,
            
            /* Linux 64 bit */
            Linux,

            /* Mac 64 bit */
            Mac
        }

        class Project
        {
            public string Name { get; set; }

            public string Version { get; set; }

            public string Configuration { get; set; }
            
            [JsonConverter(typeof(StringEnumConverter))]
            public EPlatform Platform { get; set; }
        }
    }
}
