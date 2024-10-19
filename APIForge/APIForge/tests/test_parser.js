const { expect } = require('chai');
const { parseApiSpecification } = require('./parser'); // Import your parser function

describe('API Specification Parser', () => {

    it('should parse valid API specification', () => {
        const apiSpecification = {
            name: "TestAPI",
            version: "1.0.0",
            endpoints: [
                {
                    method: "GET",
                    path: "/test",
                    description: "Test endpoint"
                }
            ]
        };

        const result = parseApiSpecification(apiSpecification);
        expect(result).to.be.an('object');
        expect(result.name).to.equal('TestAPI');
        expect(result.version).to.equal('1.0.0');
        expect(result.endpoints).to.have.lengthOf(1);
        expect(result.endpoints[0]).to.include({
            method: 'GET',
            path: '/test',
            description: 'Test endpoint'
        });
    });

    it('should throw an error for invalid API specification', () => {
        const invalidApiSpecification = {
            name: "TestAPI",
            version: "1.0.0",
            endpoints: [
                {
                    method: "INVALID_METHOD",
                    path: "/test",
                    description: "Test endpoint"
                }
            ]
        };

        expect(() => parseApiSpecification(invalidApiSpecification)).to.throw(Error, 'Invalid HTTP method: INVALID_METHOD');
    });

    it('should throw an error for empty API specification', () => {
        const emptyApiSpecification = {};

        expect(() => parseApiSpecification(emptyApiSpecification)).to.throw(Error, 'API specification cannot be empty');
    });
});
