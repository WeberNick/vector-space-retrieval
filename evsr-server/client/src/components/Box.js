import React, { Component } from 'react';

class Box extends Component {
  truncate = string => {
    if (string.length > 100) return string.substring(0, 300) + '...';
    else return string;
  };

  render() {
    const { doc } = this.props;

    return (
      <div className="box" style={{ marginBottom: '0px' }}>
        <article className="media">
          <div className="media-content">
            <div className="content">
              <p>
                <strong>{doc.title}</strong> <small>@Similarity</small> :{' '}
                <small>{doc.similarity}</small>
                <br />
                {this.truncate(doc.content)}
              </p>
            </div>
          </div>
        </article>
      </div>
    );
  }
}

export default Box;
